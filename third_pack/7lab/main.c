#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

void print_usage(const char* program_name) {
    printf("Использование:\n");
    printf("  %s <input_file> [log_file]\n", program_name);
    printf("  %s interactive [log_file]\n", program_name);
    printf("\nПримеры:\n");
    printf("  %s program.txt trace.log\n", program_name);
    printf("  %s interactive\n", program_name);
}

void interactive_mode(InterpreterState* state) {
    printf("=== ИНТЕРАКТИВНЫЙ РЕЖИМ ИНТЕРПРЕТАТОРА ===\n\n");
    printf("Поддерживаемые команды:\n");
    printf("  A = 5          - присваивание\n");
    printf("  B = A + 3 * 2  - арифметические операции\n");
    printf("  C = A ^ B      - возведение в степень\n");
    printf("  print(A)       - вывод значения\n");
    printf("  exit           - выход\n\n");
    
    char line[MAX_LINE_LENGTH];
    
    while (1) {
        printf("> ");
        
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "exit") == 0 || strcmp(line, "quit") == 0) {
            break;
        }
        
        InterpreterStatus status = process_line(state, line);
        if (status != SUCCESS) {
            printf("Ошибка: ");
            switch (status) {
                case ERROR_INVALID_SYNTAX:
                    printf("неверный синтаксис\n");
                    break;
                case ERROR_UNKNOWN_VARIABLE:
                    printf("неизвестная переменная\n");
                    break;
                case ERROR_DIVISION_BY_ZERO:
                    printf("деление на ноль\n");
                    break;
                case ERROR_INVALID_EXPRESSION:
                    printf("неверное выражение\n");
                    break;
                default:
                    printf("неизвестная ошибка\n");
                    break;
            }
        }
    }
    
    printf("\nЗавершение интерактивного режима.\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    const char* log_filename = "trace.log";
    if (argc >= 3) {
        log_filename = argv[2];
    }
    
    InterpreterState state;
    InterpreterStatus status = initialize_interpreter(&state, log_filename);
    if (status != SUCCESS) {
        fprintf(stderr, "Ошибка инициализации интерпретатора\n");
        return 1;
    }
    
    printf("Интерпретатор математических выражений\n");
    printf("Файл трассировки: %s\n\n", log_filename);
    
    if (strcmp(argv[1], "interactive") == 0) {
        interactive_mode(&state);
    } else {
        status = process_file(&state, argv[1]);
        if (status != SUCCESS) {
            fprintf(stderr, "Ошибка обработки файла\n");
        } else {
            printf("Файл успешно обработан.\n");
        }
    }
    
    printf("\n");
    print_variables(&state);
    
    cleanup_interpreter(&state);
    
    printf("\nТрассировка сохранена в %s\n", log_filename);
    return 0;
}
