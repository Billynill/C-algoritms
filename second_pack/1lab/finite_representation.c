#include "finite_representation.h"
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

static StatusCode is_finite_representation(double number, int base, double epsilon, bool *result) {
    if (result == NULL) {
        return STATUS_ERROR_NULL_POINTER;
    }
    
    if (base < 2) {
        return STATUS_ERROR_INVALID_BASE;
    }
    
    if (number <= 0.0 || number >= 1.0) {
        return STATUS_ERROR_INVALID_NUMBER_RANGE;
    }
    
    if (epsilon <= 0.0) {
        return STATUS_ERROR_INVALID_EPSILON;
    }

    double temp = number;
    long long denominator = 1;
    const int max_iterations = 15;
    
    for (int i = 0; i < max_iterations; i++) {
        if (fabs(temp - round(temp)) < epsilon) {
            break;
        }
        temp *= 10.0;
        denominator *= 10;
        
        if (denominator < 0) {
            *result = false;
            return STATUS_SUCCESS;
        }
    }
    
    long long numerator = (long long)(temp + 0.5);
    
    long long a = numerator;
    long long b = denominator;
    
    while (b != 0) {
        long long remainder = a % b;
        a = b;
        b = remainder;
    }
    
    long long gcd = a;
    denominator /= gcd;
    
    int temp_denom = (int)denominator;
    for (int i = 2; i <= temp_denom; i++) {
        if (temp_denom % i == 0) {
            bool factor_found = false;
            int temp_base = base;
            
            while (temp_base > 1) {
                if (temp_base % i == 0) {
                    factor_found = true;
                    break;
                }
                
                int remainder = temp_base % i;
                if (remainder == 0) {
                    factor_found = true;
                    break;
                }
                temp_base = remainder;
            }
            
            if (!factor_found) {
                *result = false;
                return STATUS_SUCCESS;
            }
            
            while (temp_denom % i == 0) {
                temp_denom /= i;
            }
        }
    }
    
    *result = true;
    return STATUS_SUCCESS;
}
StatusCode check_finite_representation(int base, double epsilon, int count, ...) {
    if (base < 2) {
        return STATUS_ERROR_INVALID_BASE;
    }
    
    if (epsilon <= 0.0) {
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
        printf("Результаты проверки для системы с основанием %d:\n", base);
        va_list args_for_output;
        va_start(args_for_output, count);
        
        for (int i = 0; i < count; i++) {
            double number = va_arg(args_for_output, double);
            printf("  Число %.10f - %s\n", number, 
                   results[i] ? "КОНЕЧНОЕ" : "БЕСКОНЕЧНОЕ");
        }
        
        va_end(args_for_output);
    }
    
    free(results);
    return overall_status;
}