#include "finite_representation.h"
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

static long long compute_gcd(long long a, long long b) {
    while (b != 0) {
        long long remainder = a % b;
        a = b;
        b = remainder;
    }
    return a;
}

StatusCode is_finite_representation(double number, int base, double epsilon, bool *result) {
    // Валидация входных параметров
    if (result == NULL) {
        return STATUS_ERROR_NULL_POINTER;
    }
    
    if (base < 2) {
        return STATUS_ERROR_INVALID_BASE;
    }
    
    if (number <= 0.0 || number >= 1.0) {
        return STATUS_ERROR_INVALID_NUMBER_RANGE;
    }
    
    if (epsilon <= 0.0 || epsilon >= 1.0) {
        return STATUS_ERROR_INVALID_EPSILON;
    }


    double temp = number;
    long long denominator = 1;
    const int max_iterations = 100;
    
    for (int i = 0; i < max_iterations; i++) {
        if (fabs(temp - round(temp)) < epsilon) {
            temp = round(temp);
            break;
        }
        temp *= 10.0;
        denominator *= 10;
        
        if (denominator < 0 || denominator > 1e15) {
            *result = false;
            return STATUS_SUCCESS;
        }
    }
    
    long long numerator = (long long)(temp + 0.5);
    
    // Сокращаем дробь
    long long gcd = compute_gcd(numerator, denominator);
    denominator /= gcd;
    
    // Проверяем, содержит ли знаменатель только простые множители основания
    long long temp_denom = denominator;
    long long temp_base = base;
    
    while (temp_base > 1) {
        long long factor = temp_base;
        for (long long i = 2; i * i <= temp_base; i++) {
            if (temp_base % i == 0) {
                factor = i;
                break;
            }
        }
        
        while (temp_denom % factor == 0) {
            temp_denom /= factor;
        }
        
        while (temp_base % factor == 0) {
            temp_base /= factor;
        }
    }
    
    *result = (temp_denom == 1);
    return STATUS_SUCCESS;
}

StatusCode check_finite_representation(int base, double epsilon, int count, ...) {
    if (base < 2) {
        return STATUS_ERROR_INVALID_BASE;
    }
    
    if (epsilon <= 0.0 || epsilon >= 1.0) {
        return STATUS_ERROR_INVALID_EPSILON;
    }
    
    if (count <= 0) {
        return STATUS_ERROR_INVALID_COUNT;
    }
    
    bool *results = (bool*)malloc(count * sizeof(bool));
    if (results == NULL) {
        return STATUS_ERROR_MEMORY_ALLOCATION;
    }
    
    StatusCode overall_status = STATUS_SUCCESS;
    va_list args;
    va_start(args, count);
    
    for (int i = 0; i < count; i++) {
        double number = va_arg(args, double);
        StatusCode status = is_finite_representation(number, base, epsilon, &results[i]);
        
        if (status != STATUS_SUCCESS && overall_status == STATUS_SUCCESS) {
            overall_status = status;
        }
    }
    
    va_end(args);
    
    if (overall_status == STATUS_SUCCESS) {
        printf("Results for base %d (epsilon = %g):\n", base, epsilon);
        va_list args_for_output;
        va_start(args_for_output, count);
        
        for (int i = 0; i < count; i++) {
            double number = va_arg(args_for_output, double);
            printf("  Number %.10f: %s\n", number, 
                   results[i] ? "FINITE" : "INFINITE");
        }
        
        va_end(args_for_output);
    }
    
    free(results);
    return overall_status;
}

void print_status_code(StatusCode status) {
    switch (status) {
        case STATUS_SUCCESS:
            printf("Success\n");
            break;
        case STATUS_ERROR_INVALID_BASE:
            printf("Error: Invalid base (must be >= 2)\n");
            break;
        case STATUS_ERROR_INVALID_NUMBER_RANGE:
            printf("Error: Number must be in range (0;1)\n");
            break;
        case STATUS_ERROR_INVALID_EPSILON:
            printf("Error: Epsilon must be in range (0;1)\n");
            break;
        case STATUS_ERROR_NULL_POINTER:
            printf("Error: Null pointer\n");
            break;
        case STATUS_ERROR_INVALID_COUNT:
            printf("Error: Invalid count (must be > 0)\n");
            break;
        case STATUS_ERROR_MEMORY_ALLOCATION:
            printf("Error: Memory allocation failed\n");
            break;
    }
}
