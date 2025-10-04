//
// Created by Рустам on 01.10.2025.
//

#ifndef INC_3LAB_VALIDATE_H
#define INC_3LAB_VALIDATE_H

#include <stdbool.h>

bool validate_epsilon(double epsilon);
bool validate_triangle_sides(double a, double b, double c, double epsilon);
bool equals_with_epsilon(double a, double b, double epsilon);
bool greater_than_with_epsilon(double a, double b, double epsilon);

#endif //INC_3LAB_VALIDATE_H
