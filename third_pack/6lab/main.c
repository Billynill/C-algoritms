#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "brackets.h"

#define MAX_INPUT_LENGTH 1000

void print_result(BracketStatus status) {
    switch (status) {
        case BRACKETS_OK: 
            printf("Скобки расставлены ПРАВИЛЬНО\n");
            break;
        case BRACKETS_UNBALANCED: 
            printf("ОШИБКА: Несбалансированные скобки\n");
            break;
        case BRACKETS_EMPTY_STRING: 
            printf("Строка пуста (скобок нет)\n");
            break;
        case BRACKETS_INVALID_POINTER: 
            printf("ОШИБКА: Неверный указатель\n");
            break;
    }
}

void interactive_mode(void) {
    printf("=== ИНТЕРАКТИВНАЯ ПРОВЕРКА СКОБОК ===\n\n");
    printf("Введите строку для проверки скобок:\n");
    printf("(поддерживаются: () [] {} <>)\n");
    printf("(для выхода введите 'exit' или нажмите Ctrl+C)\n\n");
    
    char input[MAX_INPUT_LENGTH];
    
    while (1) {
        printf("> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nЗавершение работы...\n");
            break;
        }
        
        // Убираем символ новой строки
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        // Проверяем команду выхода
        if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
            printf("Выход из программы...\n");
            break;
        }
        
        // Проверяем скобки
        printf("Проверяем: \"%s\"\n", input);
        BracketStatus result = check_brackets(input);
        print_result(result);
        printf("\n");
    }
}

void demo_examples(void) {
    printf("=== ПРИМЕРЫ ДЛЯ ТЕСТИРОВАНИЯ ===\n\n");
    
    const char *examples[] = {
        "(a + b) * c",
        "array[i] + object{value}",
        "if (x > 0) { y = arr[i]; }",
        "((({[]})))",
        "function(a, [b, {c: d}])",
        "([)]",
        "({[}])",
        "((((()",
        "))))",
        "normal text without brackets",
        "",
        "<html><body></body></html>",
        "((1 + 2) * [3 - {4 / 5}])",
        NULL
    };
    
    for (int i = 0; examples[i] != NULL; i++) {
        printf("Пример %d: \"%s\"\n", i + 1, examples[i]);
        BracketStatus result = check_brackets(examples[i]);
        print_result(result);
        printf("\n");
    }
}

int main(void) {
    printf("=================================\n");
    printf("    ПРОВЕРКА СКОБОК v1.0\n");
    printf("=================================\n\n");
    
    int choice;
    printf("Выберите режим:\n");
    printf("1. Интерактивная проверка (ввод текста)\n");
    printf("2. Демонстрационные примеры\n");
    printf("Ваш выбор: ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Неверный ввод!\n");
        return 1;
    }
    
    // Очищаем буфер после scanf
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    switch (choice) {
        case 1:
            interactive_mode();
            break;
        case 2:
            demo_examples();
            break;
        default:
            printf("Неверный выбор! Запуск интерактивного режима...\n\n");
            interactive_mode();
            break;
    }
    
    printf("\nЗавершение\n");
    return 0;
}
