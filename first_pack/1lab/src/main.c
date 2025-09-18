#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "logic.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Использование: %s <число> <флаг>\n", argv[0]);
        printf("Флаги: -h, -a, -f, -p, -s, -e\n");
        return 1;
    }

    if (argv[2][0] != '-' && argv[2][0] != '/') {
        printf("Ошибка: флаг должен начинаться с - или /\n");
        return 1;
    }

    int x = atoi(argv[1]);
    if (x == 0 && argv[1][0] != '0') {
        printf("Ошибка: некорректное число\n");
        return 1;
    }

    if (strcmp(argv[2], "-h") == 0) {
        int numbers[100];
        int count = 0;
        StatusCode status = find_multiples(x, numbers, &count);

        switch (status) {
            case SUCCESS:
                printf("Числа кратные %d: ", x);
                for (int i = 0; i < count; i++) {
                    printf("%d ", numbers[i]);
                }
                printf("\n");
                break;
            case ERROR_NO_MULTIPLES:
                printf("Чисел кратных %d в диапазоне 1-100 нет\n", x);
                break;
            case ERROR_INVALID_INPUT:
                printf("Ошибка: число должно быть в диапазоне 1-100\n");
                break;
            default:
                printf("Неизвестная ошибка\n");
        }
    }
    else if (strcmp(argv[2], "-a") == 0) {
        long sum = 0;
        StatusCode status = calculate_sum(x, &sum);

        switch (status) {
            case SUCCESS:
                printf("Сумма чисел от 1 до %d: %ld\n", x, sum);
                break;
            case ERROR_INVALID_INPUT:
                printf("Ошибка: число должно быть натуральным\n");
                break;
            default:
                printf("Неизвестная ошибка\n");
        }
    }
    else if (strcmp(argv[2], "-f") == 0) {
        long long factorial = 0;
        StatusCode status = calculate_factorial(x, &factorial);

        switch (status) {
            case SUCCESS:
                printf("Факториал %d: %lld\n", x, factorial);
                break;
            case ERROR_NEGATIVE_NUMBER:
                printf("Ошибка: факториал определен только для неотрицательных чисел\n");
                break;
            case ERROR_OVERFLOW:
                printf("Ошибка: переполнение при вычислении факториала\n");
                break;
            default:
                printf("Неизвестная ошибка\n");
        }
    }
    else if (strcmp(argv[2], "-p") == 0) {
        bool is_prime_result = false;
        StatusCode status = is_prime(x, &is_prime_result);

        switch (status) {
            case SUCCESS:
                if (is_prime_result) {
                    printf("%d - простое число\n", x);
                } else {
                    printf("%d - составное число\n", x);
                }
                break;
            case ERROR_INVALID_INPUT:
                printf("Ошибка: некорректные данные\n");
                break;
            default:
                printf("Неизвестная ошибка\n");
        }
    }
    else if (strcmp(argv[2], "-s") == 0) {
        char result[100];
        int length = 0;
        StatusCode status = split_hex_digits(x, result, &length);

        switch (status) {
            case SUCCESS:
                printf("16-ричные цифры %d: %s\n", x, result);
                break;
            case ERROR_NEGATIVE_NUMBER:
                printf("Ошибка: число не должно быть отрицательным\n");
                break;
            case ERROR_INVALID_INPUT:
                printf("Ошибка: некорректные данные\n");
                break;
            default:
                printf("Неизвестная ошибка\n");
        }
    }
    else if (strcmp(argv[2], "-e") == 0) {
        long long table[10][10];
        StatusCode status = calculate_power_table(x, table);

        switch (status) {
            case SUCCESS:
                printf("Основание |");
                for (int power = 1; power <= x; power++) {
                    printf(" Степень %d |", power);
                }
                printf("\n---------|");
                for (int power = 1; power <= x; power++) {
                    printf("-----------|");
                }
                printf("\n");

                for (int base = 0; base < 10; base++) {
                    printf("%9d |", base + 1);
                    for (int power = 0; power < x; power++) {
                        printf("%11lld |", table[base][power]);
                    }
                    printf("\n");
                }
                break;
            case ERROR_INVALID_INPUT:
                printf("Ошибка: x должен быть в диапазоне 1-10\n");
                break;
            case ERROR_OVERFLOW:
                printf("Ошибка: переполнение при вычислении степеней\n");
                break;
            default:
                printf("Неизвестная ошибка\n");
        }
    }
    else {
        printf("Неизвестный флаг: %s\n", argv[2]);
        printf("Доступные флаги: -h, -a, -f, -p, -s, -e\n");
        return 1;
    }

    return 0;
}