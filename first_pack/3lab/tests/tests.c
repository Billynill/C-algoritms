//
// Created by Рустам on 9/21/25.
//

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../src/math_oper.h"
#include "../src/validate.h"

// Тестовые функции для валидации
void test_validation(void) {
    printf("=== Testing Validation ===\n");

    // Test validate_epsilon
    printf("Test validate_epsilon: ");
    if (validate_epsilon(0.001) && !validate_epsilon(-0.001) && !validate_epsilon(0)) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    // Test validate_triangle_sides
    printf("Test validate_triangle_sides: ");
    if (validate_triangle_sides(3, 4, 5, 0.001) &&
        !validate_triangle_sides(1, 1, 3, 0.001) &&
        !validate_triangle_sides(-1, 2, 3, 0.001)) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    // Test equals_with_epsilon
    printf("Test equals_with_epsilon: ");
    if (equals_with_epsilon(1.0, 1.0000001, 0.001) &&
        !equals_with_epsilon(1.0, 1.1, 0.001) &&
        equals_with_epsilon(5.0, 5.0, 0.001)) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    // Test greater_than_with_epsilon
    printf("Test greater_than_with_epsilon: ");
    if (greater_than_with_epsilon(2.0, 1.0, 0.001) &&
        !greater_than_with_epsilon(1.0, 1.0, 0.001) &&
        !greater_than_with_epsilon(0.5, 1.0, 0.001)) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    printf("\n");
}

// Тестовые функции для математических операций
void test_quadratic_equation(void) {
    printf("=== Testing Quadratic Equation ===\n");

    QuadraticSolution solution;

    // Test two real roots: x^2 - 3x + 2 = 0 -> x = 1, x = 2
    printf("Test two real roots (x^2 - 3x + 2 = 0): ");
    solve_quadratic(1, -3, 2, 0.0001, &solution);
    if (solution.roots_count == 2) {
        // Check if roots are approximately 1 and 2 (order may vary)
        int root1_ok = (fabs(solution.x1 - 1.0) < 0.001) || (fabs(solution.x1 - 2.0) < 0.001);
        int root2_ok = (fabs(solution.x2 - 1.0) < 0.001) || (fabs(solution.x2 - 2.0) < 0.001);
        int different_roots = fabs(solution.x1 - solution.x2) > 0.001;
        if (root1_ok && root2_ok && different_roots) {
            printf("PASS\n");
        } else {
            printf("FAIL (roots: %.6f, %.6f)\n", solution.x1, solution.x2);
        }
    } else {
        printf("FAIL (got %d roots)\n", solution.roots_count);
    }

    // Test one real root: x^2 - 2x + 1 = 0 -> x = 1
    printf("Test one real root (x^2 - 2x + 1 = 0): ");
    solve_quadratic(1, -2, 1, 0.0001, &solution);
    if (solution.roots_count == 1 && fabs(solution.x1 - 1.0) < 0.001) {
        printf("PASS\n");
    } else {
        printf("FAIL (roots: %d, x1: %.6f)\n", solution.roots_count, solution.x1);
    }

    // Test no real roots: x^2 + 1 = 0
    printf("Test no real roots (x^2 + 1 = 0): ");
    solve_quadratic(1, 0, 1, 0.0001, &solution);
    if (solution.roots_count == 0) {
        printf("PASS\n");
    } else {
        printf("FAIL (roots: %d)\n", solution.roots_count);
    }

    // Test linear equation: 2x - 4 = 0 -> x = 2
    printf("Test linear equation (2x - 4 = 0): ");
    solve_quadratic(0, 2, -4, 0.0001, &solution);
    if (solution.roots_count == 1 && fabs(solution.x1 - 2.0) < 0.001) {
        printf("PASS\n");
    } else {
        printf("FAIL (roots: %d, x1: %.6f)\n", solution.roots_count, solution.x1);
    }

    // Test no solution linear: 0x + 5 = 0
    printf("Test no solution linear (0x + 5 = 0): ");
    solve_quadratic(0, 0, 5, 0.0001, &solution);
    if (solution.roots_count == 0) {
        printf("PASS\n");
    } else {
        printf("FAIL (roots: %d)\n", solution.roots_count);
    }

    // Test infinite solutions linear: 0x + 0 = 0
    printf("Test infinite solutions linear (0x + 0 = 0): ");
    solve_quadratic(0, 0, 0, 0.0001, &solution);
    if (solution.roots_count == -1) {
        printf("PASS\n");
    } else {
        printf("FAIL (roots: %d)\n", solution.roots_count);
    }

    printf("\n");
}

void test_multiple_check(void) {
    printf("=== Testing Multiple Check ===\n");

    // Test multiples
    printf("Test positive multiple (10, 5): ");
    if (check_multiple(10, 5) == SUCCESS) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    printf("Test negative multiple (-15, 3): ");
    if (check_multiple(-15, 3) == SUCCESS) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    printf("Test non-multiple (10, 3): ");
    if (check_multiple(10, 3) == SUCCESS) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    printf("Test zero multiple (0, 5): ");
    if (check_multiple(0, 5) == SUCCESS) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    printf("\n");
}

void test_right_triangle(void) {
    printf("=== Testing Right Triangle ===\n");

    // Test right triangle 3-4-5
    printf("Test 3-4-5 triangle: ");
    if (check_right_triangle(0.0001, 3, 4, 5) == SUCCESS) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    // Test right triangle with different order
    printf("Test 3-4-5 triangle (different order): ");
    if (check_right_triangle(0.0001, 5, 3, 4) == SUCCESS) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    // Test non-right triangle
    printf("Test non-right triangle (2, 3, 4): ");
    if (check_right_triangle(0.0001, 2, 3, 4) == SUCCESS) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    // Test another right triangle 5-12-13
    printf("Test 5-12-13 triangle: ");
    if (check_right_triangle(0.0001, 5, 12, 13) == SUCCESS) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    printf("\n");
}

void test_error_cases(void) {
    printf("=== Testing Error Cases ===\n");

    // Test invalid triangle sides (triangle inequality)
    printf("Test invalid triangle sides (1, 1, 3): ");
    if (validate_triangle_sides(1, 1, 3, 0.001) == false) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    // Test zero division in multiple check
    printf("Test zero division: ");
    if (check_multiple(10, 0) == ZERO_DIVISION) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    // Test negative sides
    printf("Test negative triangle sides: ");
    if (validate_triangle_sides(-1, 2, 3, 0.001) == false) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    printf("\n");
}

void test_quadratic_permutations(void) {
    printf("=== Testing Quadratic Permutations ===\n");

    printf("Test permutations with 1, -3, 2: ");
    if (solve_quadratic_permutations(0.0001, 1, -3, 2) == SUCCESS) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    printf("Test permutations with 1, 0, -4 (should have real roots): ");
    if (solve_quadratic_permutations(0.0001, 1, 0, -4) == SUCCESS) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    printf("\n");
}

void run_all_tests(void) {
    printf("Running all tests...\n\n");

    test_validation();
    test_quadratic_equation();
    test_multiple_check();
    test_right_triangle();
    test_error_cases();
    test_quadratic_permutations();

    printf("All tests completed!\n");
}

int main(void) {
    run_all_tests();
    return 0;
}
