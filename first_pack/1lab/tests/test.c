//
// Created by Рустам on 9/16/25.
//

#include "../src/logic.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void test_find_multiples() {
    printf("Тестирование поиска кратных значений...\n");

    int results[100];
    int count;

    // Тест 1: Кратные 5
    count = find_multiples(5, results);
    assert(count == 20);
    assert(results[0] == 5);
    assert(results[19] == 100);

    // Тест 2: Нет кратных
    count = find_multiples(101, results);
    assert(count == 0);

    printf("find_multiples тест пройден!\n");
}

void test_calculate_sum() {
    printf("Тестирование calculate_sum...\n");

    assert(calculate_sum(5) == 15);
    assert(calculate_sum(10) == 55);
    assert(calculate_sum(100) == 5050);

    printf("calculate_sum тесты пройдены!\n");
}

void test_calculate_factorial() {
    printf("Тестирование calculate_factorial...\n");

    assert(calculate_factorial(5) == 120);
    assert(calculate_factorial(0) == 1);
    assert(calculate_factorial(1) == 1);

    printf("calculate_factorial тест пройден!\n");
}

void test_is_prime() {
    printf("Тестирование is_prime...\n");

    assert(is_prime(2) == true);
    assert(is_prime(3) == true);
    assert(is_prime(4) == false);
    assert(is_prime(17) == true);

    printf("is_prime тест пройден!\n");
}

void test_split_hex_digits() {
    printf("Тестирование split_hex_digits...\n");

    char result[100];

    split_hex_digit(255, result);
    assert(strcmp(result, "F F") == 0);

    split_hex_digit(0, result);
    assert(strcmp(result, "0") == 0);

    printf("split_hex_digits тесты пройдены!\n");
}

int main() {
    printf("Начало тестов...\n\n");

    test_find_multiples();
    test_calculate_sum();
    test_calculate_factorial();
    test_is_prime();
    test_split_hex_digits();

    printf("\nВсе тесты пройдены! ✅\n");
    return 0;
}