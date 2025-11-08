#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "overprintf.h"

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void test_roman(void) {
    printf("\n=== %%Ro - Римские цифры ===\n");
    int number;
    
    printf("Введите целое число (1-3999): ");
    scanf("%d", &number);
    clear_input_buffer();
    
    char buffer[100];
    overfprintf(stdout, "Результат: %Ro\n", number);
    
    oversprintf(buffer, "Римские цифры: %Ro", number);
    printf("В строке: %s\n", buffer);
}

void test_zeckendorf(void) {
    printf("\n=== %%Zr - Цекендорфово представление ===\n");
    unsigned int number;
    
    printf("Введите неотрицательное целое число: ");
    scanf("%u", &number);
    clear_input_buffer();
    
    char buffer[100];
    overfprintf(stdout, "Результат: %Zr\n", number);
    
    oversprintf(buffer, "Цекендорф: %Zr", number);
    printf("В строке: %s\n", buffer);
}

void test_custom_base(void) {
    printf("\n=== %%Cv/%%CV - Системы счисления ===\n");
    int number, base;
    char choice;
    
    printf("Введите целое число: ");
    scanf("%d", &number);
    clear_input_buffer();
    
    printf("Введите основание системы (2-36): ");
    scanf("%d", &base);
    clear_input_buffer();
    
    printf("Регистр (u - верхний, l - нижний): ");
    scanf("%c", &choice);
    clear_input_buffer();
    
    char buffer[100];
    if (choice == 'u' || choice == 'U') {
        overfprintf(stdout, "Результат: %CV\n", number, base);
        oversprintf(buffer, "Верхний регистр: %CV", number, base);
    } else {
        overfprintf(stdout, "Результат: %Cv\n", number, base);
        oversprintf(buffer, "Нижний регистр: %Cv", number, base);
    }
    printf("В строке: %s\n", buffer);
}

void test_to_decimal(void) {
    printf("\n=== %%to/%%TO - В десятичную систему ===\n");
    char str[100];
    int base;
    char choice;
    
    printf("Введите число в строковом виде: ");
    scanf("%99s", str);
    clear_input_buffer();
    
    printf("Введите исходное основание системы (2-36): ");
    scanf("%d", &base);
    clear_input_buffer();
    
    printf("Регистр (u - верхний, l - нижний): ");
    scanf("%c", &choice);
    clear_input_buffer();
    
    char buffer[100];
    if (choice == 'u' || choice == 'U') {
        overfprintf(stdout, "Результат: %TO\n", str, base);
        oversprintf(buffer, "Верхний регистр: %TO", str, base);
    } else {
        overfprintf(stdout, "Результат: %to\n", str, base);
        oversprintf(buffer, "Нижний регистр: %to", str, base);
    }
    printf("В строке: %s\n", buffer);
}

void test_memory_dump_int(void) {
    printf("\n=== %%mi - Дампа памяти int ===\n");
    int number;
    
    printf("Введите целое число: ");
    scanf("%d", &number);
    clear_input_buffer();
    
    char buffer[200];
    overfprintf(stdout, "Дампа: %mi\n", number);
    
    oversprintf(buffer, "Дампа int: %mi", number);
    printf("В строке: %s\n", buffer);
}

void test_memory_dump_uint(void) {
    printf("\n=== %%mu - Дампа памяти unsigned int ===\n");
    unsigned int number;
    
    printf("Введите неотрицательное целое число: ");
    scanf("%u", &number);
    clear_input_buffer();
    
    char buffer[200];
    overfprintf(stdout, "Дампа: %mu\n", number);
    
    oversprintf(buffer, "Дампа unsigned int: %mu", number);
    printf("В строке: %s\n", buffer);
}

void test_memory_dump_float(void) {
    printf("\n=== %%mf - Дампа памяти float ===\n");
    float number;
    
    printf("Введите вещественное число: ");
    scanf("%f", &number);
    clear_input_buffer();
    
    char buffer[200];
    overfprintf(stdout, "Дампа: %mf\n", number);
    
    oversprintf(buffer, "Дампа float: %mf", number);
    printf("В строке: %s\n", buffer);
}

void test_memory_dump_double(void) {
    printf("\n=== %%md - Дампа памяти double ===\n");
    double number;
    
    printf("Введите вещественное число: ");
    scanf("%lf", &number);
    clear_input_buffer();
    
    char buffer[200];
    overfprintf(stdout, "Дампа: %md\n", number);
    
    oversprintf(buffer, "Дампа double: %md", number);
    printf("В строке: %s\n", buffer);
}

int main(void) {
    int choice;
    
    do {
        printf("\n=== ТЕСТИРОВАНИЕ OVERPRINTF ===\n");
        printf("1. %%Ro  - Римские цифры\n");
        printf("2. %%Zr  - Цекендорфово представление\n");
        printf("3. %%Cv/V - Система счисления\n");
        printf("4. %%to/O - В десятичную систему\n");
        printf("5. %%mi  - Дампа памяти int\n");
        printf("6. %%mu  - Дампа памяти unsigned int\n");
        printf("7. %%mf  - Дампа памяти float\n");
        printf("8. %%md  - Дампа памяти double\n");
        printf("0. Выход\n");
        printf("Выберите тест: ");
        
        scanf("%d", &choice);
        clear_input_buffer();
        
        switch (choice) {
            case 1: test_roman(); break;
            case 2: test_zeckendorf(); break;
            case 3: test_custom_base(); break;
            case 4: test_to_decimal(); break;
            case 5: test_memory_dump_int(); break;
            case 6: test_memory_dump_uint(); break;
            case 7: test_memory_dump_float(); break;
            case 8: test_memory_dump_double(); break;
            case 0: printf("Выход...\n"); break;
            default: printf("Неверный выбор!\n");
        }
        
        if (choice != 0) {
            printf("\nНажмите Enter для продолжения...");
            getchar();
        }
        
    } while (choice != 0);
    
    return 0;
}