#pragma once

#include <stdint.h>
#include <string.h>
#include <vector>

class Client
{
    /**
     * @brief Максимальный размер буфера клиента
     */
    static constexpr uint8_t maxBufSize = 128;

    int sd = -1;    /*!< Сокет дескриптор клиента */
    char buf[maxBufSize]; /*!< Буфер для записи полученных данных */

public:

    /**
     * @brief Состояние клиента
     */
    enum class State
    {
        undef,  /*!< Состояние не определено */
        accept, /*!< Ожидает подключения */
        read,   /*!< Ожидает получения данных */
        answer, /*!< Ожидает отправки ответа */
    };

    /**
     * @brief Установить сокет дескриптор клиента
     * @param [in] sd Сокет дескриптор
     */
    void setSd (int sd) { this->sd = sd; }

    /**
     * @brief Получить сокет дескриптор клиента
     * @return сокет дескриптор клиента 
     */
    int getSd () const { return sd; }

    /**
     * @brief Очистить буфер
     */
    void clearBuffer () { memset (buf, 0, sizeof (buf)); }

    /**
     * @brief Установить новое состояние клиента
     * @param [in] state Новое состояние
     */
    void setState (State state) { this->state = state; }

    /**
     * @brief Получить состояние клиента
     * @return Состояние клиента 
     */
    State getState () const { return state; }

    /**
     * @brief Получить указатель на буфер
     * @return Указатель на буфер 
     */
    char* getBufPtr () { return buf; }

    /**
     * @brief Получить размер буфера
     * @return Размер буфера, байт
     */
    uint8_t getBufSize () const { return sizeof (buf); }

private:

    State state = State::undef; /*!< Состояние клиента */
};

/**
 * @brief Класс управления клиентами
 */
class ClientControl
{
    std::vector<Client*>clients; /*!< Вектор указателей созданных клиентов */
    
public:

    /**
     * @brief Создать клиента
     * @param [in] sd Сокет дескриптор клиента
     * @return Указатель на клиента
     */
    Client* createClient (int sd);

    /**
     * @brief Найти клиента по сокет дескриптору
     * @param [in] sd Сокет дескриптор клиента
     * @return Указатель на найденного клиента
     * @retval nullptr если клиент не найден
     */
    Client* findClient (int sd);

    /**
     * @brief Удалить клиента
     * @param [in] sd Сокет дескриптор клиента
     */
    void deleteClient (int sd);
};
