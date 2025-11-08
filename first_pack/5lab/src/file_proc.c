#include "file_proc.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 4096

FlagType parse_flag_type(const char* flag) {
    if (flag == NULL) return FLAG_D;
    
    // Извлекаем основной флаг (игнорируя 'n' если есть)
    char main_flag = '\0';
    for (int i = 0; flag[i] != '\0'; i++) {
        if (flag[i] == 'd' || flag[i] == 'i' || flag[i] == 's' || flag[i] == 'a') {
            main_flag = flag[i];
            break;
        }
    }
    
    switch (main_flag) {
        case 'd': return FLAG_D;
        case 'i': return FLAG_I;
        case 's': return FLAG_S;
        case 'a': return FLAG_A;
        default: return FLAG_D;
    }
}

ProcessStatusCode determine_output_path(const char* input_path, const char* flag, char** output_path) {
    if (input_path == NULL || flag == NULL || output_path == NULL) {
        return PROCESS_ERROR_INVALID_ARGS;
    }
    
    // Проверяем есть ли 'n' как второй символ
    if (flag[1] == 'n') {
        // Выходной файл должен быть третьим аргументом
        // Эта логика будет в main
        return PROCESS_SUCCESS;
    } else {
        // Генерируем имя файла
        *output_path = generate_output_filename(input_path);
        if (*output_path == NULL) {
            return PROCESS_ERROR_MEMORY;
        }
    }
    
    return PROCESS_SUCCESS;
}

ProcessStatusCode process_flag_d(FILE* input, FILE* output) {
    char buffer[BUFFER_SIZE];
    
    while (fgets(buffer, BUFFER_SIZE, input) != NULL) {
        for (char* p = buffer; *p != '\0'; p++) {
            if (!is_arabic_digit(*p)) {
                fputc(*p, output);
            }
        }
    }
    
    return PROCESS_SUCCESS;
}

ProcessStatusCode process_flag_i(FILE* input, FILE* output) {
    char buffer[BUFFER_SIZE];
    
    while (fgets(buffer, BUFFER_SIZE, input) != NULL) {
        int count = 0;
        for (char* p = buffer; *p != '\0'; p++) {
            if (is_latin_letter(*p)) {
                count++;
            }
        }
        fprintf(output, "%d\n", count);
    }
    
    return PROCESS_SUCCESS;
}

ProcessStatusCode process_flag_s(FILE* input, FILE* output) {
    char buffer[BUFFER_SIZE];
    
    while (fgets(buffer, BUFFER_SIZE, input) != NULL) {
        int count = 0;
        for (char* p = buffer; *p != '\0'; p++) {
            if (is_special_char(*p)) {
                count++;
            }
        }
        fprintf(output, "%d\n", count);
    }
    
    return PROCESS_SUCCESS;
}

ProcessStatusCode process_flag_a(FILE* input, FILE* output) {
    char buffer[BUFFER_SIZE];
    
    while (fgets(buffer, BUFFER_SIZE, input) != NULL) {
        for (char* p = buffer; *p != '\0'; p++) {
            if (is_arabic_digit(*p)) {
                fputc(*p, output);
            } else {
                fprintf(output, "%02X", (unsigned char)*p);
            }
        }
    }
    
    return PROCESS_SUCCESS;
}

ProcessStatusCode process_file(const char* input_path, const char* output_path, FlagType flag_type) {
    if (input_path == NULL || output_path == NULL) {
        return PROCESS_ERROR_INVALID_ARGS;
    }
    
    FILE* input = fopen(input_path, "r");
    if (input == NULL) {
        return PROCESS_ERROR_FILE_OPEN;
    }
    
    FILE* output = fopen(output_path, "w");
    if (output == NULL) {
        fclose(input);
        return PROCESS_ERROR_FILE_OPEN;
    }
    
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
    
    fclose(input);
    fclose(output);
    
    return result;
}