#include <fstream>

#pragma once

/**
 * @brief Класс для работы с файлами
 */
class File
{
    std::fstream file; /*!< Поток для чтения/записи файла */

public:

    /**
     * @brief Открыть файл
     * @param [in] filename Указатель на строку с названием файла (с нуль терминатором в конце)
     * @retval true Файл открыт
     * @retval false Не удалось открыть файл
     */
    bool open (const char* filename);

    /**
     * @brief Записать данные в файл
     * @param [in] data Указатель на данные для записи 
     * @param [in] size Размер данных для записи, байт
     * @retval true Данные записаны
     * @retval false Не удалось записать данные
     */
    bool write (char* data, size_t size);

    /**
     * @brief Закрыть файл 
     */
    void close ();
};
