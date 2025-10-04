//
// Created by Рустам on 9/21/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math_oper.h"
#include "validate.h"

void print_usage(void) {
    printf("Usage:\n");
    printf("  -q <epsilon> <a> <b> <c> - Решит квадратное уравнение со всеми перестановками\n");
    printf("  -m <num1> <num2> - Проверть, кратно ли первое число второму\n");
    printf("  -t <epsilon> <side1> <side2> <side3> - Проверить, образуют ли стороны прямоугольный треугольник\n");
}

StatusCode parse_double(const char* str, double* value) {
    if (str == NULL || value == NULL) return INVALID_NUMBER_FORMAT;

    char* endptr;
    *value = strtod(str, &endptr);
    if (endptr == str || *endptr != '\0') {
        return INVALID_NUMBER_FORMAT;
    }
    return SUCCESS;
}

StatusCode parse_int(const char* str, int* value) {
    if (str == NULL || value == NULL) return INVALID_NUMBER_FORMAT;

    char* endptr;
    long result = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0' || result < -2147483648L || result > 2147483647L) {
        return INVALID_NUMBER_FORMAT;
    }
    *value = (int)result;
    return SUCCESS;
}

StatusCode handle_q_flag(int argc, char* argv[]) {
    if (argc != 6) return INVALID_ARGS_COUNT;

    double epsilon, a, b, c;
    StatusCode status;

    if ((status = parse_double(argv[2], &epsilon)) != SUCCESS) return status;
    if ((status = parse_double(argv[3], &a)) != SUCCESS) return status;
    if ((status = parse_double(argv[4], &b)) != SUCCESS) return status;
    if ((status = parse_double(argv[5], &c)) != SUCCESS) return status;

    if (!validate_epsilon(epsilon)) return INVALID_EPSILON;

    return solve_quadratic_permutations(epsilon, a, b, c);
}

StatusCode handle_m_flag(int argc, char* argv[]) {
    if (argc != 4) return INVALID_ARGS_COUNT;

    int num1, num2;
    StatusCode status;

    if ((status = parse_int(argv[2], &num1)) != SUCCESS) return status;
    if ((status = parse_int(argv[3], &num2)) != SUCCESS) return status;

    if (num2 == 0) return ZERO_DIVISION;

    return check_multiple(num1, num2);
}

StatusCode handle_t_flag(int argc, char* argv[]) {
    if (argc != 6) return INVALID_ARGS_COUNT;

    double epsilon, side1, side2, side3;
    StatusCode status;

    if ((status = parse_double(argv[2], &epsilon)) != SUCCESS) return status;
    if ((status = parse_double(argv[3], &side1)) != SUCCESS) return status;
    if ((status = parse_double(argv[4], &side2)) != SUCCESS) return status;
    if ((status = parse_double(argv[5], &side3)) != SUCCESS) return status;

    if (!validate_epsilon(epsilon)) return INVALID_EPSILON;
    if (!validate_triangle_sides(side1, side2, side3, epsilon)) {
        printf("Ошибка: Эти стороны не могут образовывать треугольник\n");
        return NO_TRIANGLE;
    }

    return check_right_triangle(epsilon, side1, side2, side3);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    const char* flag = argv[1];
    StatusCode status = SUCCESS;

    if (strcmp(flag, "-q") == 0 || strcmp(flag, "/q") == 0) {
        status = handle_q_flag(argc, argv);
    } else if (strcmp(flag, "-m") == 0 || strcmp(flag, "/m") == 0) {
        status = handle_m_flag(argc, argv);
    } else if (strcmp(flag, "-t") == 0 || strcmp(flag, "/t") == 0) {
        status = handle_t_flag(argc, argv);
    } else {
        status = INVALID_FLAG;
    }

    if (status != SUCCESS) {
        switch (status) {
            case INVALID_ARGS_COUNT:
                printf("Ошибка: Неверное количество аргументов\n");
                print_usage();
                break;
            case INVALID_FLAG:
                printf("Ошибка: Недопустимый флаг\n");
                print_usage();
                break;
            case INVALID_NUMBER_FORMAT:
                printf("Ошибка: Неверный формат номера\n");
                break;
            case INVALID_EPSILON:
                printf("Ошибка: Эпсилон должен быть положительным и достаточно малым\n");
                break;
            case ZERO_DIVISION:
                printf("Ошибка: деление на ноль \n");
                break;
            case NO_TRIANGLE:
                // Сообщение об ошибке, уже напечатанное в handle_t_flag
                break;
            case MEMORY_ERROR:
                printf("Ошибка: произошла ошибка памяти\n");
                break;
            default:
                printf("шибка: произошла неизвестная ошибка\n");
                break;
        }
        return status;
    }

    return 0;
}
