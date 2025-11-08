#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_proc.h"
#include "utils.h"

void print_usage(const char* program_name) {
    printf("Usage:\n");
    printf("  %s -d <input_file> [output_file]\n", program_name);
    printf("  %s -i <input_file> [output_file]\n", program_name);
    printf("  %s -s <input_file> [output_file]\n", program_name);
    printf("  %s -a <input_file> [output_file]\n", program_name);
    printf("  %s -nd <input_file> <output_file>\n", program_name);
    printf("  %s -ni <input_file> <output_file>\n", program_name);
    printf("  %s -ns <input_file> <output_file>\n", program_name);
    printf("  %s -na <input_file> <output_file>\n", program_name);
    printf("\nFlags:\n");
    printf("  -d  Remove Arabic digits\n");
    printf("  -i  Count Latin letters per line\n");
    printf("  -s  Count special characters per line\n");
    printf("  -a  Replace non-digits with hex codes\n");
}

int main(int argc, char* argv[]) {
    // Валидация количества аргументов
    if (argc < 2) {
        print_usage(argv[0]);
        return PROCESS_ERROR_INVALID_ARGS;
    }
    
    const char* flag = argv[1];
    
    // Валидация флага
    if (flag[0] != '-' && flag[0] != '/') {
        printf("Error: Flag must start with '-' or '/'\n");
        print_usage(argv[0]);
        return PROCESS_ERROR_INVALID_FLAG;
    }
    
    // Определяем тип флага и количество ожидаемых аргументов
    FlagType flag_type = parse_flag_type(flag);
    bool has_n = (flag[1] == 'n');
    
    const char* input_path = NULL;
    const char* output_path = NULL;
    char* generated_output_path = NULL;
    
    ProcessStatusCode status;
    
    // Обработка аргументов в зависимости от наличия 'n'
    if (has_n) {
        if (argc != 4) {
            printf("Error: With -n* flags, both input and output files must be specified\n");
            print_usage(argv[0]);
            return PROCESS_ERROR_INVALID_ARGS;
        }
        input_path = argv[2];
        output_path = argv[3];
    } else {
        if (argc < 3) {
            printf("Error: Input file must be specified\n");
            print_usage(argv[0]);
            return PROCESS_ERROR_INVALID_ARGS;
        }
        input_path = argv[2];
        
        if (argc == 4) {
            output_path = argv[3];
        } else {
            // Генерируем имя выходного файла
            status = determine_output_path(input_path, flag, &generated_output_path);
            if (status != PROCESS_SUCCESS) {
                printf("Error: Could not generate output filename\n");
                return status;
            }
            output_path = generated_output_path;
        }
    }
    
    // Обрабатываем файл
    status = process_file(input_path, output_path, flag_type);
    
    // Освобождаем память если генерировали имя файла
    if (generated_output_path != NULL) {
        free(generated_output_path);
    }
    
    // Обработка ошибок
    switch (status) {
        case PROCESS_SUCCESS:
            printf("File processed successfully: %s -> %s\n", input_path, output_path);
            break;
        case PROCESS_ERROR_FILE_OPEN:
            printf("Error: Could not open file\n");
            break;
        case PROCESS_ERROR_FILE_READ:
            printf("Error: Could not read file\n");
            break;
        case PROCESS_ERROR_FILE_WRITE:
            printf("Error: Could not write to file\n");
            break;
        case PROCESS_ERROR_MEMORY:
            printf("Error: Memory allocation failed\n");
            break;
        default:
            printf("Error: Processing failed\n");
            break;
    }
    
    return status;
}