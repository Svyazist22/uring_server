#include <stdint.h>

#include <netinet/in.h>
#include <sys/socket.h>

#pragma once

class Server
{
private:

    int sd = -1;        /*!< Сокет дескриптор */
    uint16_t port;      /*!< Порт для прослушивания */
    sockaddr_in sockAddress; /*!< Адрес сокета */

public:
    /**
     * @brief Конструктор по умолчанию удален
     */
    Server () = delete;

    /**
     * @brief Конструктор с параметром
     * @param [in] port Порт для прослушивания сервером
     */
    Server (uint16_t port) : port (port) {}

    /**
     * @brief Создать сервер
     * @retval true Сервер успешно создан
     * @retval false Не удалось создать сервер
     */
    bool create ();

    /**
     * @brief Получить сокет дескриптор
     * @return Сокет дескриптор, полученный при создании
     * @retval Если сервер не создан, то -1
     */
    int getSd () const { return sd; }

    /**
     * @brief Получить структуру адреса сокета
     * @return Структура адреса сокета
     */
    sockaddr_in getSockAddress () const { return sockAddress; }
};
