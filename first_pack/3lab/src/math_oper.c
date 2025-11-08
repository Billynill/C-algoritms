//
// Created by Рустам on 01.10.2025.
//

#include "math_oper.h"
#include "validate.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

StatusCode solve_quadratic(double a, double b, double c, double epsilon, QuadraticSolution* solution) {
    if (solution == NULL) return MEMORY_ERROR;

    memset(solution, 0, sizeof(QuadraticSolution));

    if (equals_with_epsilon(a, 0, epsilon)) {
        if (equals_with_epsilon(b, 0, epsilon)) {
            if (equals_with_epsilon(c, 0, epsilon)) {
                solution->roots_count = -1;
            } else {
                solution->roots_count = 0;
            }
        } else {
            solution->x1 = -c / b;
            solution->roots_count = 1;
        }
        return SUCCESS;
    }

    double discriminant = b * b - 4 * a * c;

    if (greater_than_with_epsilon(discriminant, 0, epsilon)) {
        // Two real roots
        double sqrt_d = sqrt(discriminant);
        solution->x1 = (-b + sqrt_d) / (2 * a);
        solution->x2 = (-b - sqrt_d) / (2 * a);
        solution->roots_count = 2;
    } else if (equals_with_epsilon(discriminant, 0, epsilon)) {
        solution->x1 = -b / (2 * a);
        solution->roots_count = 1;
    } else {
        solution->roots_count = 0;
    }

    return SUCCESS;
}

void print_solution(const QuadraticSolution* solution) {
    if (solution == NULL) return;

    switch (solution->roots_count) {
        case -1:
            printf("Бесконечные решения");
            break;
        case 0:
            printf("Нету настоящих корней");
            break;
        case 1:
            printf("Первый корень: x = %.6f", solution->x1);
            break;
        case 2:
            printf("Второй корень: x1 = %.6f, x2 = %.6f", solution->x1, solution->x2);
            break;
        default:
            printf("Неизвестный тип решения");
            break;
    }
}

StatusCode solve_quadratic_permutations(double epsilon, double a, double b, double c) {
    double coefficients[3] = {a, b, c};
    int permutations[6][3] = {
        {0, 1, 2}, {0, 2, 1}, {1, 0, 2},
        {1, 2, 0}, {2, 0, 1}, {2, 1, 0}
    };

    printf("Решение квадратных уравнений со всеми перестановками коэффициентов:\n");
    printf("Коеффициенты: a=%.6f, b=%.6f, c=%.6f, epsilon=%.6f\n\n", a, b, c, epsilon);

    for (int i = 0; i < 6; i++) {
        double perm_a = coefficients[permutations[i][0]];
        double perm_b = coefficients[permutations[i][1]];
        double perm_c = coefficients[permutations[i][2]];

        printf("Перестановка %d: a=%.6f, b=%.6f, c=%.6f -> ", i + 1, perm_a, perm_b, perm_c);

        QuadraticSolution solution;
        StatusCode status = solve_quadratic(perm_a, perm_b, perm_c, epsilon, &solution);
        if (status != SUCCESS) return status;

        print_solution(&solution);
        printf("\n");
    }

    return SUCCESS;
}

StatusCode check_multiple(int num1, int num2) {
    if (num2 == 0) return ZERO_DIVISION;

    if (num1 % num2 == 0) {
        printf("%d является кратным %d\n", num1, num2);
    } else {
        printf("%d не является кратным %d\n", num1, num2);
    }

    return SUCCESS;
}

StatusCode check_right_triangle(double epsilon, double side1, double side2, double side3) {
    double sides[3] = {side1, side2, side3};

    for (int i = 0; i < 2; i++) {
        for (int j = i + 1; j < 3; j++) {
            if (greater_than_with_epsilon(sides[i], sides[j], epsilon)) {
                double temp = sides[i];
                sides[i] = sides[j];
                sides[j] = temp;
            }
        }
    }

    double c = sides[2];
    double a = sides[0];
    double b = sides[1];

    // Check Pythagorean theorem: a² + b² = c²
    double left = a * a + b * b;
    double right = c * c;

    if (equals_with_epsilon(left, right, epsilon)) {
        printf("Стороны %.6f, %.6f, %.6f формируют правильный треугольник\n", side1, side2, side3);
    } else {
        printf("Стороны %.6f, %.6f, %.6f НЕ формируют правильный треугольник\n", side1, side2, side3);
    }

    return SUCCESS;
}
