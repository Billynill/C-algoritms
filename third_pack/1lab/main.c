#include <stdio.h>
#include "dec_to_base.h"

void run_tests(void);

int main(void) {
    printf("=== Тестирование перевода чисел в системы счисления 2^r ===\n\n");
    
    run_tests();
    
    return 0;
}

void run_tests(void) {
    // Тест 1: базовые случаи
    printf("--- Базовые тесты ---\n");
    print_conversion(10, 1);  
    print_conversion(10, 2);  
    print_conversion(10, 3);  
    print_conversion(10, 4);  
    print_conversion(10, 5);  
    printf("\n");
    
    // Тест 2: граничные значения
    printf("--- Граничные значения ---\n");
    print_conversion(0, 3);   
    print_conversion(1, 3);   
    print_conversion(255, 3); 
    print_conversion(256, 3); 
    printf("\n");
    
    // Тест 3: разные основания для одного числа
    printf("--- Число 42 в разных системах ---\n");
    print_conversion(42, 1); 
    print_conversion(42, 2); 
    print_conversion(42, 3); 
    print_conversion(42, 4); 
    printf("\n");
    
    // Тест 4: большие числа
    printf("--- Большие числа ---\n");
    print_conversion(1024, 3);    
    print_conversion(65535, 4);  
    print_conversion(123456789, 4); 
    printf("\n");
    
    // Тест 5: некорректные параметры
    printf("--- Тесты валидации ---\n");
    print_conversion(10, 0);  
    print_conversion(10, 6);  
    
    // Тест с малым буфером
    char small_buffer[3];
    status_code result = dec_to_base_r(100, 3, small_buffer, 3);
    if (result == BUFFER_TOO_SMALL) {
        printf("Ожидаемая ошибка: буфер слишком мал для числа 100\n");
    }
}
