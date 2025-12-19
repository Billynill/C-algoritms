#ifndef FILE_PROCESSOR_H
#define FILE_PROCESSOR_H

#include <stdbool.h>

// Коды статуса выполнения
typedef enum {
    PROCESS_SUCCESS = 0,
    PROCESS_ERROR_INVALID_ARGS,
    PROCESS_ERROR_INVALID_FLAG,
    PROCESS_ERROR_FILE_OPEN,
    PROCESS_ERROR_FILE_READ,
    PROCESS_ERROR_FILE_WRITE,
    PROCESS_ERROR_MEMORY
} ProcessStatusCode;

// Типы флагов
typedef enum {
    FLAG_INVALID = -1,
    FLAG_D = 0,  // -d исключить цифры
    FLAG_I,      // -i подсчет букв
    FLAG_S,      // -s подсчет специальных символов  
    FLAG_A       // -a замена на hex
} FlagType;

// Основная функция обработки файла
ProcessStatusCode process_file(const char* input_path, const char* output_path, FlagType flag_type);

// Определение типа флага
FlagType parse_flag_type(const char* flag);

// Генерация имени выходного файла
char* generate_output_filename(const char* input_filename);

#endif
