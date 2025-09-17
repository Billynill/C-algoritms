//
// Created by Рустам on 9/16/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "logic.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Использование: %s <число> <флаг>\n", argv[0]);
        return 1;
    }
    if (argv[2][0] != '-' && argv[2][0] != '/') {
        printf("Ошибка: флаг должен начинаться с - или /\n");
        return 1;
    }

    int x = atoi(argv[1]);
    if (x == 0) {
        printf("Ошибка: не правильное число \n");
        return 1;
    }

    if (strcmp(argv[2], "-h") == 0) {
        int numbers[100];
        int count = find_multiples(x, numbers);

        if (count == 0) {
            printf("Чисел кратных %d в диапазоне 1-100 нет\n", x);
        } else {
            printf("Числа кратные %d: ", x);
            for (int i = 0; i < count; i++) {
                printf("%d ", numbers[i]);
            }
            printf("\n");
        }
    } else if (strcmp(argv[2], "-a") == 0) {
        long sum = calculate_sum(x);
        printf("Сумма чисел от 1 до %d: %ld\n", x, sum);
    } else if (strcmp(argv[2], "-f") == 0) {
        if (x < 0) {
            printf("Ошибка факториал определен только для неотрицательных чисел\n");
            return 1;
        }

        long long factorial = calculate_factorial(x);

        if (factorial == -1) {
            printf("Ошибка: некорректные данные для факториала\n");
            return 1;
        }

        printf("Факториал %d:%lld\n",x,  factorial);
    } else if (strcmp(argv[2], "-p") == 0) {
        if (x < 0) {
            printf("Ошибка: число должно быть натуральным \n");
            return 1;
        }
        bool prime = is_prime(x);
        if (prime) {
            printf("%d простое число\n", x );
        } else {
            printf("%d составное число\n", x );
        }
    } else if (strcmp(argv[2], "-s") == 0) {
        if (x < 0) {
            printf("Ошибка: число не должно быть отрицательным\n");
            return 1;
        }
        char result[100];
        int length = split_hex_digit(x, result);

        if (length < 0) {
            printf("Ошибка при обработке числа\n");
            return 1;
        }
        printf("16-ричные цифры %d: %s\n", x, result);
    } else if (strcmp(argv[2], "-e") == 0) {
        if (x <=0 || x > 10) {
            printf ("Ошибка х должен быть меньши либо равно 10\n");
            return 1;
        }

        print_power_table(x);
    }
    else {
        printf("Неизвестный флаг: %s\n", argv[2]);
        return 1;
    }

    return 0;
}