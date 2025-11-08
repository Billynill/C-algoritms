//
// Created by Рустам on 01.10.2025.
//

#ifndef INC_3LAB_MATH_OPER_H
#define INC_3LAB_MATH_OPER_H

typedef enum {
    SUCCESS = 0,
    INVALID_ARGS_COUNT,
    INVALID_FLAG,
    INVALID_NUMBER_FORMAT,
    INVALID_EPSILON,
    ZERO_DIVISION,
    NO_TRIANGLE,
    MEMORY_ERROR
} StatusCode;

typedef struct {
    double x1;
    double x2;
    int roots_count; 
} QuadraticSolution;

StatusCode solve_quadratic_permutations(double epsilon, double a, double b, double c);
StatusCode check_multiple(int num1, int num2);
StatusCode check_right_triangle(double epsilon, double side1, double side2, double side3);
StatusCode solve_quadratic(double a, double b, double c, double epsilon, QuadraticSolution* solution);
void print_solution(const QuadraticSolution* solution);

#endif //INC_3LAB_MATH_OPER_H
