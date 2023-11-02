#include <liburing.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <string>

#include "client.h"

#pragma once

/**
 * @brief Класс управления процессами
 */
class ProcessControl
{
    io_uring_params params; /*!< Параметры конфигурации */
    io_uring ring;          /*!< Интерфейс для работы */

    int sd;                 /*!< Сокет дескриптор созданного сервера */
    sockaddr* sockAddr;     /*!< Указатель на структура адреса сокета созданного сервера */
    socklen_t* sockLen;     /*!< Указатель на структуру размер сокета созданного сервера */

    ClientControl clientControl; /*!< Объект для управления клиентами */

    std::string filename = "port.txt";  /*!< Название файла для вывода */

    /**
     * @brief Информация о клиенте, ожидающем ответа
     */
    struct ClientsForAnswer
    {
        Client* client = nullptr; /*!< Указатель на клиента */
        uint64_t timestamp = 0; /*!< Временная метка начала отсчета времени, сек */
    };

    std::vector<ClientsForAnswer*> clientsForAnswer; /*!< Вектор указателей клиентов для отправки ответа */

    /**
     * @brief Добавить клиента для отправки ответа
     * @param [in] client Указатель на клиента 
     */
    void addClientForAnswer (Client* client);

    /**
     * @brief Проверить клиентов, ожидающих ответа и при необходимости создать событие для отправки 
     * ответа
     */
    void checkClientsForAnswer ();

    /**
     * @brief Добавить обработку события подключения клиента
     * @param [in] sd Сокет дескриптор
     */
    void addAccept (int sd);

    /**
     * @brief Добавить обработку события получения сообщения от клиента
     * @param [in] sd Сокет дескриптор
     */
    void addReceive (int sd);

    /**
     * @brief Добавить обработку события отправки ответа клиенту
     * @param [in] sd Сокет дескриптор
     */
    void addAnswer (int sd);

public:

    /**
     * @brief Конструктор по умолчанию удален
     */
    ProcessControl () = delete;

    /**
     * @brief Конструктор с параметрами
     * @param [in] sd Сокет дескриптор созданного сервера
     * @param [in] sockAddr Указатель на структура адреса сокета созданного сервера
     * @param [in] sockLen Указатель на структуру размер сокета созданного сервера
     */
    ProcessControl (int sd, sockaddr* sockAddr, socklen_t* sockLen);

    /**
     * @brief Запустить обработчик процессов
     */
    void run ();

    /**
     * @brief Установить название файла для записи полученных данных
     * @note Расширение файла будет иметь тип .txt
     * @param [in] filename Название файла
     */
    void setFilename (std::string filename);
};
