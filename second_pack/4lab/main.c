#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "overscanf.h"

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void test_roman_input(void) {
    printf("\n=== %%Ro - Ввод римских цифр ===\n");
    
    char input[100];
    int value;
    
    printf("Введите римское число (например, XIV): ");
    scanf("%99s", input);
    clear_input_buffer();
    
    // Тестируем oversscanf
    int result = oversscanf(input, "%Ro", &value);
    if (result == 1) {
        printf("oversscanf: распознано %d\n", value);
    } else {
        printf("oversscanf: ошибка распознавания\n");
    }
    
    // Тестируем из файла
    FILE *temp = tmpfile();
    if (temp) {
        fprintf(temp, "%s", input);
        rewind(temp);
        
        result = overfscanf(temp, "%Ro", &value);
        if (result == 1) {
            printf("overfscanf: распознано %d\n", value);
        } else {
            printf("overfscanf: ошибка распознавания\n");
        }
        fclose(temp);
    }
}

void test_zeckendorf_input(void) {
    printf("\n=== %%Zr - Ввод цекендорфова представления ===\n");
    
    char input[100];
    unsigned int value;
    
    printf("Введите цекендорфово представление (например, 101001): ");
    scanf("%99s", input);
    clear_input_buffer();
    
    // Тестируем oversscanf
    int result = oversscanf(input, "%Zr", &value);
    if (result == 1) {
        printf("oversscanf: распознано %u\n", value);
    } else {
        printf("oversscanf: ошибка распознавания\n");
    }
    
    // Тестируем из файла
    FILE *temp = tmpfile();
    if (temp) {
        fprintf(temp, "%s", input);
        rewind(temp);
        
        result = overfscanf(temp, "%Zr", &value);
        if (result == 1) {
            printf("overfscanf: распознано %u\n", value);
        } else {
            printf("overfscanf: ошибка распознавания\n");
        }
        fclose(temp);
    }
}

void test_custom_base_input(void) {
    printf("\n=== %%Cv/%%CV - Ввод из произвольной системы ===\n");
    
    char input[100];
    int value, base;
    char choice;
    
    printf("Введите число: ");
    scanf("%99s", input);
    clear_input_buffer();
    
    printf("Введите основание системы (2-36): ");
    scanf("%d", &base);
    clear_input_buffer();
    
    printf("Регистр (u - верхний, l - нижний): ");
    scanf("%c", &choice);
    clear_input_buffer();
    
    // Тестируем oversscanf
    int result;
    if (choice == 'u' || choice == 'U') {
        result = oversscanf(input, "%CV", &value, base);
        printf("oversscanf с %%CV: ");
    } else {
        result = oversscanf(input, "%Cv", &value, base);
        printf("oversscanf с %%Cv: ");
    }
    
    if (result == 1) {
        printf("распознано %d\n", value);
    } else {
        printf("ошибка распознавания\n");
    }
    
    // Тестируем из файла
    FILE *temp = tmpfile();
    if (temp) {
        fprintf(temp, "%s", input);
        rewind(temp);
        
        if (choice == 'u' || choice == 'U') {
            result = overfscanf(temp, "%CV", &value, base);
            printf("overfscanf с %%CV: ");
        } else {
            result = overfscanf(temp, "%Cv", &value, base);
            printf("overfscanf с %%Cv: ");
        }
        
        if (result == 1) {
            printf("распознано %d\n", value);
        } else {
            printf("ошибка распознавания\n");
        }
        fclose(temp);
    }
}

void test_mixed_input(void) {
    printf("\n=== Смешанный ввод ===\n");
    
    char input[200];
    int num1, num2;
    unsigned int num3;
    char str[50];
    
    printf("Введите данные в формате: <римское> <число_в_2_системе> <обычное_число> <строка>\n");
    printf("Пример: XIV 10101 42 Hello\n");
    printf("Ваш ввод: ");
    
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; // Убираем перевод строки
    
    // Тестируем oversscanf со смешанными спецификаторами
    int result = oversscanf(input, "%Ro %Cv %u %s", &num1, &num2, 2, &num3, str);
    
    if (result == 4) {
        printf("Успешно распознано:\n");
        printf("  Римское число: %d\n", num1);
        printf("  Двоичное число: %d\n", num2);
        printf("  Беззнаковое число: %u\n", num3);
        printf("  Строка: %s\n", str);
    } else {
        printf("Распознано %d из 4 полей\n", result);
    }
}

void test_complex_input(void) {
    printf("\n=== Комплексный ввод ===\n");
    
    char input[200];
    int decimal_num, hex_num, binary_num;
    unsigned int zeckendorf_num;
    
    printf("Введите данные в формате: <10_система> <16_система> <2_система> <цекендорф>\n");
    printf("Пример: 255 FF 1101 101001\n");
    printf("Ваш ввод: ");
    
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    int result = oversscanf(input, "%d %CV %Cv %Zr", &decimal_num, &hex_num, 16, &binary_num, 2, &zeckendorf_num);
    
    if (result == 4) {
        printf("Успешно распознано:\n");
        printf("  Десятичное: %d\n", decimal_num);
        printf("  Шестнадцатеричное: %d (0x%X)\n", hex_num, hex_num);
        printf("  Двоичное: %d\n", binary_num);
        printf("  Цекендорф: %u\n", zeckendorf_num);
    } else {
        printf("Распознано %d из 4 полей\n", result);
    }
}

void test_file_input(void) {
    printf("\n=== Ввод из файла ===\n");
    
    char filename[100];
    printf("Введите имя файла для тестирования (оставьте пустым для создания тестового): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;
    
    FILE *file;
    if (strlen(filename) == 0) {
        file = fopen("test_input.txt", "w");
        if (file) {
            fprintf(file, "XIV 10101 255 FF\n");
            fprintf(file, "XLII 1001 16 1A\n");
            fclose(file);
            file = fopen("test_input.txt", "r");
            printf("Создан тестовый файл 'test_input.txt'\n");
        }
    } else {
        file = fopen(filename, "r");
    }
    
    if (file == NULL) {
        printf("Ошибка открытия файла\n");
        return;
    }
    
    char line[200];
    int line_num = 1;
    
    printf("\nЧтение из файла:\n");
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        
        int roman, binary, decimal, hex;
        int result = oversscanf(line, "%Ro %Cv %d %CV", &roman, &binary, 2, &decimal, &hex, 16);
        
        printf("Строка %d: ", line_num++);
        if (result == 4) {
            printf("Рим:%d Двоич:%d Десят:%d Шест:%d\n", roman, binary, decimal, hex);
        } else {
            printf("распознано %d/4 полей\n", result);
        }
    }
    
    fclose(file);
}

int main(void) {
    int choice;
    
    do {
        printf("\n=== ТЕСТИРОВАНИЕ OVERSCANF ===\n");
        printf("1. %%Ro  - Ввод римских цифр\n");
        printf("2. %%Zr  - Ввод цекендорфова представления\n");
        printf("3. %%Cv/V - Ввод из произвольной системы\n");
        printf("4. Смешанный ввод\n");
        printf("5. Комплексный ввод\n");
        printf("6. Ввод из файла\n");
        printf("0. Выход\n");
        printf("Выберите тест: ");
        
        scanf("%d", &choice);
        clear_input_buffer();
        
        switch (choice) {
            case 1: test_roman_input(); break;
            case 2: test_zeckendorf_input(); break;
            case 3: test_custom_base_input(); break;
            case 4: test_mixed_input(); break;
            case 5: test_complex_input(); break;
            case 6: test_file_input(); break;
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