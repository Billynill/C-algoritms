#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "text_formatter.h"

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void demonstrate_example() {
    printf("\n=== Пример форматирования ===\n");
    
    // Создаем тестовый входной файл
    FILE *test_input = fopen("example_input.txt", "w");
    if (test_input) {
        fprintf(test_input, "Это очень длинная строка текста, которая должна быть разбита на несколько строк по 80 символов с правильным переносом слов и равномерным распределением пробелов между словами.\n");
        fprintf(test_input, "Короткая строка.\n");
        fprintf(test_input, "Еще одна очень длинная строка с множеством слов разной длины для демонстрации работы алгоритма форматирования текста по ширине в 80 символов.\n");
        fclose(test_input);
        printf("Создан тестовый файл 'example_input.txt'\n");
    }
    
    // Форматируем файл
    if (format_file("example_input.txt", "example_output.txt")) {
        printf("Результат сохранен в 'example_output.txt'\n");
        
        // Показываем результат
        printf("\nСодержимое выходного файла:\n");
        printf("============================\n");
        
        FILE *output = fopen("example_output.txt", "r");
        if (output) {
            char line[100];
            int line_num = 1;
            while (fgets(line, sizeof(line), output)) {
                printf("%3d: %s", line_num++, line);
            }
            fclose(output);
        }
    }
}

void interactive_mode() {
    printf("\n=== Интерактивный режим ===\n");
    
    char input_path[256];
    char output_path[256];
    
    printf("Введите путь к входному файлу: ");
    fgets(input_path, sizeof(input_path), stdin);
    input_path[strcspn(input_path, "\n")] = '\0'; // Убираем перевод строки
    
    printf("Введите путь к выходному файлу: ");
    fgets(output_path, sizeof(output_path), stdin);
    output_path[strcspn(output_path, "\n")] = '\0';
    
    if (strlen(input_path) == 0 || strlen(output_path) == 0) {
        printf("Ошибка: пути к файлам не могут быть пустыми\n");
        return;
    }
    
    printf("\nНачинаем форматирование...\n");
    printf("Входной файл: %s\n", input_path);
    printf("Выходной файл: %s\n", output_path);
    
    if (format_file(input_path, output_path)) {
        printf("\nФорматирование успешно завершено!\n");
        
        // Показываем первые несколько строк результата
        printf("\nПервые 10 строк результата:\n");
        printf("===========================\n");
        
        FILE *output = fopen(output_path, "r");
        if (output) {
            char line[100];
            int line_num = 1;
            while (fgets(line, sizeof(line), output) && line_num <= 10) {
                printf("%3d: %s", line_num++, line);
            }
            fclose(output);
            
            if (line_num > 10) {
                printf("... (файл продолжается)\n");
            }
        }
    }
}

void create_test_file() {
    printf("\n=== Создание тестового файла ===\n");
    
    char filename[256];
    printf("Введите имя тестового файла: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        strcpy(filename, "test_input.txt");
    }
    
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Ошибка создания файла\n");
        return;
    }
    
    printf("Введите текст (пустая строка для завершения):\n");
    
    char line[1000];
    while (1) {
        printf("> ");
        fgets(line, sizeof(line), stdin);
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) {
            break;
        }
        
        fprintf(file, "%s\n", line);
    }
    
    fclose(file);
    printf("Файл '%s' создан успешно\n", filename);
}

int main(void) {
    int choice;
    
    printf("=== ПРОГРАММА ФОРМАТИРОВАНИЯ ТЕКСТА ===\n");
    printf("Форматирует текст по ширине 80 символов с переносом слов\n\n");
    
    do {
        printf("Меню:\n");
        printf("1. Интерактивный режим (ввод путей к файлам)\n");
        printf("2. Демонстрационный пример\n");
        printf("3. Создать тестовый файл\n");
        printf("0. Выход\n");
        printf("Выберите действие: ");
        
        scanf("%d", &choice);
        clear_input_buffer();
        
        switch (choice) {
            case 1:
                interactive_mode();
                break;
            case 2:
                demonstrate_example();
                break;
            case 3:
                create_test_file();
                break;
            case 0:
                printf("Выход из программы...\n");
                break;
            default:
                printf("Неверный выбор!\n");
        }
        
        if (choice != 0) {
            printf("\nНажмите Enter для продолжения...");
            getchar();
        }
        
    } while (choice != 0);
    
    return 0;
}