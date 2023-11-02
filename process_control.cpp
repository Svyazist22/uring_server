#include "process_control.h"

#include <string.h>
#include <iostream>

#include "file.h"

ProcessControl::ProcessControl (int sd, sockaddr* sockAddr, socklen_t* sockLen)
{
    static constexpr uint16_t maxEntries = 1024;

    memset (&params, 0, sizeof (params));
    memset (&ring, 0, sizeof (ring));

    io_uring_queue_init_params (maxEntries, &ring, &params);

    this->sd = sd;
    this->sockAddr = sockAddr;
    this->sockLen = sockLen;
}

void ProcessControl::addClientForAnswer (Client* client)
{
    auto cl = new ClientsForAnswer;
    cl->client = client;
    cl->timestamp = time (nullptr);
    client->setState (Client::State::undef);
    clientsForAnswer.push_back (cl);
}

void ProcessControl::checkClientsForAnswer ()
{
    static constexpr uint8_t delay = 3; /*!< Задержка отправки ответа, сек */
    for (size_t i = 0; i < clientsForAnswer.size (); i++)
    {
        auto cl = clientsForAnswer[i];
        if (time (nullptr) - cl->timestamp >= 3)
        {
            cl->client->setState (Client::State::answer);
            addAnswer (cl->client->getSd ());
            clientsForAnswer.erase (clientsForAnswer.begin() + i);
        }
    }
}

void ProcessControl::addAccept (int sd)
{
    // Получить указатель на доступный sqe
    io_uring_sqe* const sqe = io_uring_get_sqe (&ring);
    // Добавить как accept
    io_uring_prep_accept (sqe, sd, sockAddr, sockLen, 0);

    // Получить указатель на клиент с которым будет взаимодействие
    Client* const cl = clientControl.createClient (sd);
    cl->setState (Client::State::accept);

    // Привязать sqe и клиента
    io_uring_sqe_set_data(sqe, cl);
}

void ProcessControl::addReceive (int sd)
{
    // Получить указатель на доступный sqe
    io_uring_sqe* const sqe = io_uring_get_sqe (&ring);

    // Получить указатель на клиент с которым будет взаимодействие
    Client* const cl = clientControl.createClient (sd);
    cl->setState (Client::State::read);

    // Добавить на прием
    io_uring_prep_recv(sqe, sd, cl->getBufPtr (), cl->getBufSize (), 0);

    // Привязать sqe и клиента
    io_uring_sqe_set_data(sqe, cl);
}

void ProcessControl::addAnswer (int sd)
{
    static constexpr char answer[] = "ACCEPTED"; /*!< Строка для ответа */

    // Получить указатель на доступный sqe
    io_uring_sqe* const sqe = io_uring_get_sqe(&ring);

    // Добавить на передачу
    io_uring_prep_send (sqe, sd, answer, strlen (answer), 0);

    // Получить указатель на клиент с которым будет взаимодействие
    Client* const cl = clientControl.findClient (sd);
    cl->setState (Client::State::answer);

    // Привязать sqe и клиента
    io_uring_sqe_set_data(sqe, cl);
}

void ProcessControl::run ()
{
    // Добавить ожидание первого клиента
    addAccept (sd);

    // Т.к. программа больше ничего не делает, то просто крутимся в цикле
    while (1) 
    {
        // Проверить, нужно ли кому-то отправить ответ (если да, то добавить событие)
        checkClientsForAnswer ();

        // Добавить все события в очередь
        io_uring_submit (&ring);

        io_uring_cqe* cqes[512]; /*!< Массив указателей cqe */
        // Получить cqe
        const auto cqe_count = io_uring_peek_batch_cqe (&ring, cqes, sizeof(cqes) / sizeof(cqes[0]));

        // Прохождение по всем cqe
        for (int i = 0; i < cqe_count; ++i) 
        {
            io_uring_cqe* const cqe = cqes[i];
            
            Client* const client = static_cast<Client*>(io_uring_cqe_get_data (cqe));
            const auto state = client->getState ();

            switch (state)
            {
                // Клиент подключен
                case Client::State::accept:
                {
                    int sock_conn_fd = cqe->res;
                    printf ("accept %d\r\n", sock_conn_fd);

                    addReceive (sock_conn_fd);
                    addAccept (sd);
                    break;
                }

                // Клиент прислал данные
                case Client::State::read:
                {
                    int bytes_read = cqe->res;
                    const int sd = client->getSd ();

                    // Данных нет - закрыть сокет и удалить клиента
                    if (bytes_read <= 0) 
                    {
                        shutdown (sd, SHUT_RDWR);
                        clientControl.deleteClient (sd);
                    } 
                    // Иначе записать в файл и ожидать возможности отправить ответ
                    else  
                    {
                        File file;
                        char* const buf = client->getBufPtr ();
                        file.open (filename.c_str ());
                        file.write (buf, strlen (buf));
                        file.close ();
                        client->clearBuffer ();
                        addClientForAnswer (client);
                    }
                    break;
                }

                // Отправлен ответ клиенту
                case Client::State::answer:
                {
                    addReceive (client->getSd ());
                    break;
                }

                default:
                    break;
            }

            // Пометить событие как выполненное
            io_uring_cqe_seen (&ring, cqe);
        }
    }
}

void ProcessControl::setFilename (std::string filename)
{
    static constexpr char fileType[] = ".txt";  /*!< Тип файла */
    if (filename.size ())
        this->filename = filename + fileType;
}
