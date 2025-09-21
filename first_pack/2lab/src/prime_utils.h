//
// Created by Рустам on 9/17/25.
//

#ifndef INC_2LAB_PRIME_UTILS_H
#define INC_2LAB_PRIME_UTILS_H

typedef enum {
    PRIME_SUCCESS = 0,
    PRIME_ERROR_INVALID_INPUT,
    PRIME_ERROR_MEMORY,
    PRIME_ERROR_OVERFLOW,
    PRIME_ERROR_INTERNAL,
} PrimeStatusCode;

PrimeStatusCode find_nth_prime(int n, long long* result);
long long estimate_upper_bound(int n);

#endif //INC_2LAB_PRIME_UTILS_H

