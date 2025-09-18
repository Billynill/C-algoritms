#include "../src/logic.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void test_find_multiples() {
    printf("Тестирование поиска кратных значений...\n");

    int results[100];
    int count;
    StatusCode status;

    status = find_multiples(5, results, &count);
    assert(status == SUCCESS);
    assert(count == 20);
    assert(results[0] == 5);
    assert(results[19] == 100);

    status = find_multiples(101, results, &count);
    assert(status == ERROR_INVALID_INPUT);

    status = find_multiples(-5, results, &count);
    assert(status == ERROR_INVALID_INPUT);

    printf("find_multiples тест пройден!\n");
}

void test_calculate_sum() {
    printf("Тестирование calculate_sum...\n");

    long result;
    StatusCode status;

    status = calculate_sum(5, &result);
    assert(status == SUCCESS);
    assert(result == 15);

    status = calculate_sum(10, &result);
    assert(status == SUCCESS);
    assert(result == 55);

    status = calculate_sum(100, &result);
    assert(status == SUCCESS);
    assert(result == 5050);

    status = calculate_sum(-5, &result);
    assert(status == ERROR_INVALID_INPUT);

    printf("calculate_sum тесты пройдены!\n");
}

void test_calculate_factorial() {
    printf("Тестирование calculate_factorial...\n");

    long long result;
    StatusCode status;

    status = calculate_factorial(5, &result);
    assert(status == SUCCESS);
    assert(result == 120);

    status = calculate_factorial(0, &result);
    assert(status == SUCCESS);
    assert(result == 1);

    status = calculate_factorial(1, &result);
    assert(status == SUCCESS);
    assert(result == 1);

    status = calculate_factorial(-5, &result);
    assert(status == ERROR_NEGATIVE_NUMBER);

    printf("calculate_factorial тест пройден!\n");
}

void test_is_prime() {
    printf("Тестирование is_prime...\n");

    bool result;
    StatusCode status;

    status = is_prime(2, &result);
    assert(status == SUCCESS);
    assert(result == true);

    status = is_prime(3, &result);
    assert(status == SUCCESS);
    assert(result == true);

    status = is_prime(4, &result);
    assert(status == SUCCESS);
    assert(result == false);

    status = is_prime(17, &result);
    assert(status == SUCCESS);
    assert(result == true);

    printf("is_prime тест пройден!\n");
}

void test_split_hex_digits() {
    printf("Тестирование split_hex_digits...\n");

    char result[100];
    int length;
    StatusCode status;

    status = split_hex_digits(255, result, &length);
    assert(status == SUCCESS);
    assert(strcmp(result, "F F") == 0);
    assert(length == 3); // "F F" = 3 символа

    status = split_hex_digits(0, result, &length);
    assert(status == SUCCESS);
    assert(strcmp(result, "0") == 0);
    assert(length == 1);

    status = split_hex_digits(-5, result, &length);
    assert(status == ERROR_NEGATIVE_NUMBER);

    printf("split_hex_digits тесты пройдены!\n");
}

void test_calculate_power_table() {
    printf("Тестирование calculate_power_table...\n");

    long long table[10][10];
    StatusCode status;

    status = calculate_power_table(3, table);
    assert(status == SUCCESS);
    assert(table[0][0] == 1);
    assert(table[1][1] == 4);
    assert(table[2][2] == 27);

    status = calculate_power_table(-1, table);
    assert(status == ERROR_INVALID_INPUT);

    status = calculate_power_table(11, table);
    assert(status == ERROR_INVALID_INPUT);

    printf("calculate_power_table тест пройден!\n");
}

int main() {
    printf("Начало тестов...\n\n");

    test_find_multiples();
    test_calculate_sum();
    test_calculate_factorial();
    test_is_prime();
    test_split_hex_digits();
    test_calculate_power_table();

    printf("\nВсе тесты пройдены! ✅\n");
    return 0;
}