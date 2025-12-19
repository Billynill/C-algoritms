#include "prime_utils.h"
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

// Оценка верхней границы для n-го простого числа
// Используется формула: p_n < n * (ln(n) + ln(ln(n))) для n >= 6
long long estimate_upper_bound(int n) {
    if (n <= 0) return 0;
    
    // Для малых значений используем известные простые числа
    if (n == 1) return 2;
    
    // Для n >= 6 используем формулу Россера-Шенфельда
    double dn = (double)n;
    double log_n = log(dn);
    double log_log_n = log(log_n);
    
    // Формула: p_n < n * (ln(n) + ln(ln(n)))
    double estimate = dn * (log_n + log_log_n);
    
    // Добавляем запас для надёжности
    estimate *= 1.3;
    
    if (estimate > (double)LLONG_MAX) {
        return 0;  // Переполнение
    }
    
    long long result = (long long)ceil(estimate);
    
    // Делаем нечётным для оптимизации решета (работаем только с нечётными)
    if (result % 2 == 0) {
        result++;
    }
    
    return result;
}

// Нахождение n-го простого числа с помощью решета Эратосфена
PrimeStatusCode find_nth_prime(int n, long long* result) {
    if (n <= 0 || result == NULL) {
        return PRIME_ERROR_INVALID_INPUT;
    }
    
    // Первые два простых числа
    if (n == 1) { *result = 2; return PRIME_SUCCESS; }
    if (n == 2) { *result = 3; return PRIME_SUCCESS; }
    
    // Определяем верхнюю границу
    long long limit = estimate_upper_bound(n);
    if (limit <= 0) {
        return PRIME_ERROR_OVERFLOW;
    }
    
    // Решето для нечётных чисел
    // Число с индексом i соответствует 2*i + 1
    long long sieve_size = (limit - 1) / 2;
    unsigned char* is_prime = (unsigned char*)malloc(sieve_size + 1);
    
    if (!is_prime) {
        return PRIME_ERROR_MEMORY;
    }
    
    // Инициализация: все нечётные числа считаем простыми
    memset(is_prime, 1, sieve_size + 1);
    
    // Решето Эратосфена
    // Проходим по всем нечётным числам до sqrt(limit)
    long long sqrt_limit = (long long)sqrt((double)limit);
    for (long long i = 1; 2 * i + 1 <= sqrt_limit; i++) {
        if (is_prime[i]) {
            long long p = 2 * i + 1;  // Простое число
            long long start = (p * p - 1) / 2;
            
            // Вычёркиваем все кратные p, начиная с p^2
            for (long long j = start; j <= sieve_size; j += p) {
                is_prime[j] = 0;
            }
        }
    }
    
    // Подсчёт простых чисел
    int count = 1;  // Уже учли число 2
    long long answer = -1;
    
    for (long long i = 1; i <= sieve_size; i++) {
        if (is_prime[i]) {
            count++;
            if (count == n) {
                answer = 2 * i + 1;
                break;
            }
        }
    }
    
    free(is_prime);
    
    if (answer < 0) {
        return PRIME_ERROR_INTERNAL;
    }
    
    *result = answer;
    return PRIME_SUCCESS;
}
