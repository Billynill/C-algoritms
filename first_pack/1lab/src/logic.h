//
// Created by Рустам on 9/16/25.
//

#ifndef INC_1_ЛАБА_LOGIC_H
#define INC_1_ЛАБА_LOGIC_H
#include <stdbool.h>
#include <limits.h>

typedef enum {
    SUCCESS,
    ERROR_INVALID_INPUT,
    ERROR_NO_MULTIPLES,
    ERROR_OVERFLOW,
    ERROR_NEGATIVE_NUMBER,
    ERROR_MEMORY
} StatusCode;

StatusCode find_multiples(const int x, int results[], int* count);
StatusCode calculate_sum(const int x, long* result);
StatusCode calculate_factorial(const int x, long long* result);
StatusCode is_prime(const int x, bool* result);
StatusCode split_hex_digits(const int x, char* result, int* length);
StatusCode calculate_power_table(const int x, long long table[10][10]);




#endif //INC_1_ЛАБА_LOGIC_H