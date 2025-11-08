#ifndef FILE_PROCESSOR_H
#define FILE_PROCESSOR_H

#include <stdbool.h>

typedef enum {
    PROCESS_SUCCESS = 0,
    PROCESS_ERROR_INVALID_ARGS,
    PROCESS_ERROR_INVALID_FLAG,
    PROCESS_ERROR_FILE_OPEN,
    PROCESS_ERROR_FILE_READ,
    PROCESS_ERROR_FILE_WRITE,
    PROCESS_ERROR_MEMORY,
    PROCESS_ERROR_INTERNAL
} ProcessStatusCode;

typedef enum {
    FLAG_D = 0,  // -d исключить цифры
    FLAG_I,      // -i подсчет букв
    FLAG_S,      // -s подсчет специальных символов  
    FLAG_A       // -a замена на hex
} FlagType;

ProcessStatusCode process_file(const char* input_path, const char* output_path, FlagType flag_type);
ProcessStatusCode determine_output_path(const char* input_path, const char* flag, char** output_path);
FlagType parse_flag_type(const char* flag);

#endif