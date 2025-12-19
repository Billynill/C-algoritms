#ifndef FINITE_REPRESENTATION_H
#define FINITE_REPRESENTATION_H

#include <stdbool.h>

typedef enum {
    STATUS_SUCCESS = 0,
    STATUS_ERROR_INVALID_BASE,
    STATUS_ERROR_INVALID_NUMBER_RANGE,
    STATUS_ERROR_INVALID_EPSILON,
    STATUS_ERROR_NULL_POINTER,
    STATUS_ERROR_INVALID_COUNT,
    STATUS_ERROR_MEMORY_ALLOCATION
} StatusCode;

// Основная функция с переменным числом аргументов
StatusCode check_finite_representation(int base, double epsilon, int count, ...);

// Вспомогательная функция для работы с массивом
StatusCode check_finite_representation_array(int base, double epsilon, int count, double *numbers);

// Функция для вывода кода статуса
void print_status_code(StatusCode status);

#endif