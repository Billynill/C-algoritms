#ifndef DEC_TO_BASE_H
#define DEC_TO_BASE_H

#include <stdint.h>

typedef enum {
    SUCCESS = 0,
    INVALID_BASE = 1,
    INVALID_NUMBER = 2,
    BUFFER_TOO_SMALL = 3
} status_code;

// Конвертация числа в систему счисления с основанием 2^r
status_code dec_to_base_r(uint32_t number, uint32_t r, char* buffer, uint32_t buffer_size);

// Вспомогательная функция для демонстрации
void print_conversion(uint32_t number, uint32_t r);

#endif
