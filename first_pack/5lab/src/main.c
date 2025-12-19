#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "file_proc.h"

// Вывод справки по использованию программы
static void print_usage(const char* program_name) {
    printf("Usage:\n");
    printf("  %s -d <input_file> [output_file]\n", program_name);
    printf("  %s -i <input_file> [output_file]\n", program_name);
    printf("  %s -s <input_file> [output_file]\n", program_name);
    printf("  %s -a <input_file> [output_file]\n", program_name);
    printf("  %s -nd <input_file> <output_file>\n", program_name);
    printf("  %s -ni <input_file> <output_file>\n", program_name);
    printf("  %s -ns <input_file> <output_file>\n", program_name);
    printf("  %s -na <input_file> <output_file>\n", program_name);
    printf("  %s /d <input_file> [output_file]\n", program_name);
    printf("  %s /i <input_file> [output_file]\n", program_name);
    printf("  %s /s <input_file> [output_file]\n", program_name);
    printf("  %s /a <input_file> [output_file]\n", program_name);
    printf("  %s /nd <input_file> <output_file>\n", program_name);
    printf("  %s /ni <input_file> <output_file>\n", program_name);
    printf("  %s /ns <input_file> <output_file>\n", program_name);
    printf("  %s /na <input_file> <output_file>\n", program_name);
    printf("\nFlags:\n");
    printf("  -d, /d  Удалить цифры из файла\n");
    printf("  -i, /i  Подсчитать буквы в каждой строке\n");
    printf("  -s, /s  Подсчитать специальные символы в каждой строке\n");
    printf("  -a, /a  Заменить не-цифры на их hex ASCII код\n");
    printf("  -n*     С этими флагами выходной файл должен быть указан\n");
}

int main(int argc, char* argv[]) {
    // Проверяем количество аргументов
    if (argc < 3) {
        print_usage(argv[0]);
        return PROCESS_ERROR_INVALID_ARGS;
    }
    
    const char* flag = argv[1];
    
    // Проверяем что флаг начинается с '-' или '/'
    if (flag[0] != '-' && flag[0] != '/') {
        printf("Error: Flag must start with '-' or '/'\n");
        print_usage(argv[0]);
        return PROCESS_ERROR_INVALID_FLAG;
    }
    
    // Определяем, содержит ли флаг 'n' как второй символ
    bool has_n = (strlen(flag) >= 2 && flag[1] == 'n');
    
    const char* input_path = NULL;
    const char* output_path = NULL;
    char* generated_output_path = NULL;
    
    // Обрабатываем аргументы в зависимости от наличия 'n'
    if (has_n) {
        // Если есть 'n', то нужны оба файла: входной и выходной
        if (argc != 4) {
            printf("Error: With %s flag, both input and output files must be specified\n", flag);
            printf("Example: %s %s input.txt output.txt\n", argv[0], flag);
            return PROCESS_ERROR_INVALID_ARGS;
        }
        input_path = argv[2];
        output_path = argv[3];
    } else {
        // Если нет 'n', то минимум 3 аргумента: программа, флаг, входной файл
        if (argc < 3) {
            printf("Error: Input file must be specified\n");
            print_usage(argv[0]);
            return PROCESS_ERROR_INVALID_ARGS;
        }
        input_path = argv[2];
        
        // Если указан выходной файл (4 аргумента) - используем его
        if (argc == 4) {
            output_path = argv[3];
        } else {
            // Иначе генерируем имя выходного файла
            generated_output_path = generate_output_filename(input_path);
            if (generated_output_path == NULL) {
                printf("Error: Could not generate output filename\n");
                return PROCESS_ERROR_MEMORY;
            }
            output_path = generated_output_path;
        }
    }
    
    // Определяем тип флага
    FlagType flag_type = parse_flag_type(flag);
    if (flag_type == FLAG_INVALID) {
        printf("Error: Invalid flag '%s'\n", flag);
        print_usage(argv[0]);
        if (generated_output_path != NULL) {
            free(generated_output_path);
        }
        return PROCESS_ERROR_INVALID_FLAG;
    }
    
    // Обрабатываем файл
    ProcessStatusCode status = process_file(input_path, output_path, flag_type);
    
    // Освобождаем память если генерировали имя файла
    if (generated_output_path != NULL) {
        free(generated_output_path);
    }
    
    // Обрабатываем результат
    switch (status) {
        case PROCESS_SUCCESS:
            printf("Success! Output written to: %s\n", output_path);
            break;
        case PROCESS_ERROR_INVALID_ARGS:
            printf("Error: Invalid arguments\n");
            break;
        case PROCESS_ERROR_INVALID_FLAG:
            printf("Error: Invalid flag\n");
            break;
        case PROCESS_ERROR_FILE_OPEN:
            printf("Error: Could not open file '%s'\n", input_path);
            break;
        case PROCESS_ERROR_FILE_READ:
            printf("Error: Could not read file '%s'\n", input_path);
            break;
        case PROCESS_ERROR_FILE_WRITE:
            printf("Error: Could not write to file '%s'\n", output_path);
            break;
        case PROCESS_ERROR_MEMORY:
            printf("Error: Memory allocation failed\n");
            break;
        default:
            printf("Error: Unknown error\n");
            break;
    }
    
    return status;
}
