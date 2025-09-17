//
// Created by Рустам on 9/16/25.
//

#include "logic.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int find_multiples(int x, int results[]) {
    int count = 0;

    if (x <= 0 || x > 100) {
        return 0;
    }

    for (int i = x; i <= 100; i += x) {
        if (i % x == 0) {
            results[count] = i;
            count++;
        }
    }

    return count;
}

long calculate_sum(int x) {
    if (x<= 0) {
        return 0;
    }

    return (long)x * (x + 1) / 2;
}

long long calculate_factorial(int x) {
    if (x < 0) {
        return -1;
    }
    if (x == 0|| x == 1) {
        return 1;
    }
    long long result = 1;
    for (int i = 2; i <= x; i++) {
        result *= i;
    }
    return result;
}

bool is_prime(int x) {
    if (x <= 1) {
        return false;
    }
    if (x == 2) {
        return true;
    }
    if (x % 2 == 0) {
        return false;
    }

    int limit = sqrt(x);
    for (int i = 3; i <= limit; i += 2) {
        if (x % i == 0) {
            return false;
        }
    }
    return true;
}

int split_hex_digit(int x, char* result) {
    if (x < 0) {
        return -1;
    }

    if (x == 0) {
        result[0] = '0';
        result[1] = '\0';
        return 1;
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
    return j;
}

void print_power_table(int x) {
    if (x <= 0 || x > 10) {
        return;
    }
    printf("Основание |");
    for (int power = 1; power <= x; power++) {
        printf("Степень %d |", power);
    }
    printf("\n");

    printf("---------|");
    for (int power = 1; power <= x; power++) {
        printf("-----------|");
    }
    printf("\n");

    for (int base = 1; base <= 10; base++) {
        printf("%3d |", base);
        for (int power = 1; power <= x; power++) {
            long long result = 1;
            for (int i = 0; i < power; i++) {
                result *= base;
            }
            printf("%11lld |", result);
        }
        printf("\n");
    }
}