//
// Created by Рустам on 01.10.2025.
//

#include "validate.h"
#include <math.h>

bool validate_epsilon(double epsilon) {
    return epsilon > 0 && epsilon < 1; // epsilon should be positive and reasonably small
}

bool validate_triangle_sides(double a, double b, double c, double epsilon) {
    if (a <= 0 || b <= 0 || c <= 0) return false;

    // Check triangle inequality with epsilon precision
    if (!greater_than_with_epsilon(a + b, c, epsilon)) return false;
    if (!greater_than_with_epsilon(a + c, b, epsilon)) return false;
    if (!greater_than_with_epsilon(b + c, a, epsilon)) return false;

    return true;
}

bool equals_with_epsilon(double a, double b, double epsilon) {
    return fabs(a - b) < epsilon;
}

bool greater_than_with_epsilon(double a, double b, double epsilon) {
    return (a - b) > epsilon;
}
