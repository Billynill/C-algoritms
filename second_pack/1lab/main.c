#include <stdio.h>
#include <stdlib.h>
#include "finite_representation.h"



int main(void) {
    
    printf("=== Работа с пользовательским вводом ===\n");
    
    int base, count;
    double epsilon;
    
    printf("Введите основание системы счисления: ");
    if (scanf("%d", &base) != 1) {
        printf("Ошибка ввода основания!\n");
        return 1;
    }
    
    printf("Введите точность вычислений: ");
    if (scanf("%lf", &epsilon) != 1) {
        printf("Ошибка ввода точности!\n");
        return 1;
    }
    
    printf("Введите количество дробей для проверки: ");
    if (scanf("%d", &count) != 1) {
        printf("Ошибка ввода количества!\n");
        return 1;
    }
    
    if (count <= 0) {
        printf("Ошибка: количество должно быть положительным\n");
        return 1;
    }
    
    double *numbers = (double*)malloc(count * sizeof(double));
    if (numbers == NULL) {
        printf("Ошибка выделения памяти!\n");
        return 1;
    }
    
    printf("Введите %d дробей в диапазоне (0;1):\n", count);
    for (int i = 0; i < count; i++) {
        printf("Дробь %d: ", i + 1);
        if (scanf("%lf", &numbers[i]) != 1) {
            printf("Ошибка ввода числа!\n");
            free(numbers);
            return 1;
        }
    }
    
    StatusCode status = check_finite_representation_array(base, epsilon, count, numbers);
    
    if (status != STATUS_SUCCESS) {
        printf("Ошибка при выполнении: ");
        print_status_code(status);
    }
    
    free(numbers);
    return 0;
}