#include "file_proc.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 4096

// Определяем тип флага из строки
FlagType parse_flag_type(const char* flag) {
    if (flag == NULL || strlen(flag) < 2) {
        return FLAG_INVALID;
    }
    
    // Пропускаем первый символ (- или /)
    const char* flag_content = flag + 1;
    
    // Если второй символ 'n', пропускаем и его
    if (flag_content[0] == 'n') {
        flag_content++;
    }
    
    // Проверяем основной флаг
    if (flag_content[0] == 'd' && flag_content[1] == '\0') {
        return FLAG_D;
    } else if (flag_content[0] == 'i' && flag_content[1] == '\0') {
        return FLAG_I;
    } else if (flag_content[0] == 's' && flag_content[1] == '\0') {
        return FLAG_S;
    } else if (flag_content[0] == 'a' && flag_content[1] == '\0') {
        return FLAG_A;
    }
    
    return FLAG_INVALID;
}

// Генерируем имя выходного файла добавляя префикс "out_"
char* generate_output_filename(const char* input_filename) {
    if (input_filename == NULL) {
        return NULL;
    }
    
    const char* prefix = "out_";
    size_t prefix_len = strlen(prefix);
    size_t input_len = strlen(input_filename);
    size_t total_len = prefix_len + input_len + 1; // +1 для '\0'
    
    char* result = (char*)malloc(total_len);
    if (result == NULL) {
        return NULL;
    }
    
    // Копируем префикс
    strcpy(result, prefix);
    // Добавляем имя входного файла
    strcat(result, input_filename);
    
    return result;
}

// Обработка флага -d: исключить цифры
static ProcessStatusCode process_flag_d(FILE* input, FILE* output) {
    if (input == NULL || output == NULL) {
        return PROCESS_ERROR_INVALID_ARGS;
    }
    
    char buffer[BUFFER_SIZE];
    
    // Читаем файл построчно
    while (fgets(buffer, BUFFER_SIZE, input) != NULL) {
        // Проходим по каждому символу строки
        for (size_t i = 0; buffer[i] != '\0'; i++) {
            // Если это не цифра - записываем в выходной файл
            if (!is_arabic_digit(buffer[i])) {
                if (fputc(buffer[i], output) == EOF) {
                    return PROCESS_ERROR_FILE_WRITE;
                }
            }
        }
    }
    
    // Проверяем не было ли ошибки чтения
    if (ferror(input)) {
        return PROCESS_ERROR_FILE_READ;
    }
    
    return PROCESS_SUCCESS;
}

// Обработка флага -i: подсчет латинских букв в каждой строке
static ProcessStatusCode process_flag_i(FILE* input, FILE* output) {
    if (input == NULL || output == NULL) {
        return PROCESS_ERROR_INVALID_ARGS;
    }
    
    char buffer[BUFFER_SIZE];
    
    // Читаем файл построчно
    while (fgets(buffer, BUFFER_SIZE, input) != NULL) {
        int count = 0;
        
        // Считаем латинские буквы в строке
        for (size_t i = 0; buffer[i] != '\0'; i++) {
            if (is_latin_letter(buffer[i])) {
                count++;
            }
        }
        
        // Записываем количество в выходной файл
        if (fprintf(output, "%d\n", count) < 0) {
            return PROCESS_ERROR_FILE_WRITE;
        }
    }
    
    // Проверяем не было ли ошибки чтения
    if (ferror(input)) {
        return PROCESS_ERROR_FILE_READ;
    }
    
    return PROCESS_SUCCESS;
}

// Обработка флага -s: подсчет специальных символов в каждой строке
static ProcessStatusCode process_flag_s(FILE* input, FILE* output) {
    if (input == NULL || output == NULL) {
        return PROCESS_ERROR_INVALID_ARGS;
    }
    
    char buffer[BUFFER_SIZE];
    
    // Читаем файл построчно
    while (fgets(buffer, BUFFER_SIZE, input) != NULL) {
        int count = 0;
        
        // Считаем специальные символы в строке
        for (size_t i = 0; buffer[i] != '\0'; i++) {
            if (is_special_char(buffer[i])) {
                count++;
            }
        }
        
        // Записываем количество в выходной файл
        if (fprintf(output, "%d\n", count) < 0) {
            return PROCESS_ERROR_FILE_WRITE;
        }
    }
    
    // Проверяем не было ли ошибки чтения
    if (ferror(input)) {
        return PROCESS_ERROR_FILE_READ;
    }
    
    return PROCESS_SUCCESS;
}

// Обработка флага -a: замена не-цифр на их hex код
static ProcessStatusCode process_flag_a(FILE* input, FILE* output) {
    if (input == NULL || output == NULL) {
        return PROCESS_ERROR_INVALID_ARGS;
    }
    
    char buffer[BUFFER_SIZE];
    
    // Читаем файл построчно
    while (fgets(buffer, BUFFER_SIZE, input) != NULL) {
        // Проходим по каждому символу строки
        for (size_t i = 0; buffer[i] != '\0' && buffer[i] != '\n'; i++) {
            if (is_arabic_digit(buffer[i])) {
                // Если это цифра - оставляем как есть
                if (fputc(buffer[i], output) == EOF) {
                    return PROCESS_ERROR_FILE_WRITE;
                }
            } else {
                // Если не цифра - заменяем на hex код (2 символа)
                unsigned char code = (unsigned char)buffer[i];
                if (fprintf(output, "%02X", code) < 0) {
                    return PROCESS_ERROR_FILE_WRITE;
                }
            }
        }
        // Добавляем перевод строки после каждой строки
        if (fputc('\n', output) == EOF) {
            return PROCESS_ERROR_FILE_WRITE;
        }
    }
    
    // Проверяем не было ли ошибки чтения
    if (ferror(input)) {
        return PROCESS_ERROR_FILE_READ;
    }
    
    return PROCESS_SUCCESS;
}

// Основная функция обработки файла
ProcessStatusCode process_file(const char* input_path, const char* output_path, FlagType flag_type) {
    // Валидация параметров
    if (input_path == NULL || output_path == NULL) {
        return PROCESS_ERROR_INVALID_ARGS;
    }
    
    if (flag_type == FLAG_INVALID) {
        return PROCESS_ERROR_INVALID_FLAG;
    }
    
    // Проверка на одинаковые файлы
    if (strcmp(input_path, output_path) == 0) {
        return PROCESS_ERROR_INVALID_ARGS;
    }
    
    // Открываем входной файл
    FILE* input = fopen(input_path, "r");
    if (input == NULL) {
        return PROCESS_ERROR_FILE_OPEN;
    }
    
    // Открываем выходной файл
    FILE* output = fopen(output_path, "w");
    if (output == NULL) {
        fclose(input);
        return PROCESS_ERROR_FILE_OPEN;
    }
    
    // Обрабатываем файл в зависимости от флага
    ProcessStatusCode result = PROCESS_SUCCESS;
    
    switch (flag_type) {
        case FLAG_D:
            result = process_flag_d(input, output);
            break;
        case FLAG_I:
            result = process_flag_i(input, output);
            break;
        case FLAG_S:
            result = process_flag_s(input, output);
            break;
        case FLAG_A:
            result = process_flag_a(input, output);
            break;
        default:
            result = PROCESS_ERROR_INVALID_FLAG;
            break;
    }
    
    // Закрываем файлы
    fclose(input);
    fclose(output);
    
    return result;
}
