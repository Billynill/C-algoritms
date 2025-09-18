#include "logic.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

StatusCode find_multiples(const int x, int results[], int* count) {
    if (x <= 0 || x > 100) {
        return ERROR_INVALID_INPUT;
    }
    if (results == NULL || count == NULL) {
        return ERROR_INVALID_INPUT;
    }

    *count = 0;
    for (int i = x; i <= 100; i += x) {
        results[*count] = i;
        (*count)++;
    }

    if (*count == 0) {
        return ERROR_NO_MULTIPLES;
    }

    return SUCCESS;
}

StatusCode calculate_sum(const int x, long* result) {
    if (x <= 0) {
        return ERROR_INVALID_INPUT;
    }
    if (result == NULL) {
        return ERROR_INVALID_INPUT;
    }

    *result = (long)x * (x + 1) / 2;
    return SUCCESS;
}

StatusCode calculate_factorial(const int x, long long* result) {
    if (x < 0) {
        return ERROR_NEGATIVE_NUMBER;
    }
    if (result == NULL) {
        return ERROR_INVALID_INPUT;
    }

    if (x == 0 || x == 1) {
        *result = 1;
        return SUCCESS;
    }

    *result = 1;
    for (int i = 2; i <= x; i++) {
        if (*result > LLONG_MAX / i) {
            return ERROR_OVERFLOW;
        }
        *result *= i;
    }

    return SUCCESS;
}

StatusCode is_prime(const int x, bool* result) {
    if (x <= 1) {
        *result = false;
        return SUCCESS;
    }
    if (result == NULL) {
        return ERROR_INVALID_INPUT;
    }

    if (x == 2) {
        *result = true;
        return SUCCESS;
    }
    if (x % 2 == 0) {
        *result = false;
        return SUCCESS;
    }

    int limit = sqrt(x);
    for (int i = 3; i <= limit; i += 2) {
        if (x % i == 0) {
            *result = false;
            return SUCCESS;
        }
    }

    *result = true;
    return SUCCESS;
}

StatusCode split_hex_digits(const int x, char* result, int* length) {
    if (x < 0) {
        return ERROR_NEGATIVE_NUMBER;
    }
    if (result == NULL || length == NULL) {
        return ERROR_INVALID_INPUT;
    }

    if (x == 0) {
        result[0] = '0';
        result[1] = '\0';
        *length = 1;
        return SUCCESS;
    }

    char hex_string[20];
    sprintf(hex_string, "%X", x);

    int j = 0;
    for (int i = 0; hex_string[i] != '\0'; i++) {
        result[j++] = hex_string[i];
        if (hex_string[i + 1] != '\0') {
            result[j++] = ' ';
        }
    }
    result[j] = '\0';
    *length = j;

    return SUCCESS;
}

StatusCode calculate_power_table(const int x, long long table[10][10]) {
    if (x <= 0 || x > 10) {
        return ERROR_INVALID_INPUT;
    }
    if (table == NULL) {
        return ERROR_INVALID_INPUT;
    }

    for (int base = 0; base < 10; base++) {
        for (int power = 0; power < x; power++) {
            long long result = 1;
            int current_power = power + 1;

            for (int i = 0; i < current_power; i++) {
                if (result > LLONG_MAX / (base + 1)) {
                    return ERROR_OVERFLOW;
                }
                result *= (base + 1);
            }
            table[base][power] = result;
        }
    }

    return SUCCESS;
}