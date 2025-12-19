#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Прототипы функций
int char_to_digit(char c);
char digit_to_char(int d);
int find_min_base(const char* num);
long long convert_to_decimal(const char* num, int base);
void remove_leading_zeros(char* str);
int is_valid_number_char(char c);
void trim(char* str);

#define MAX_LINE_LEN 1024

int main(int argc, char* argv[]) {
    // Проверяем аргументы командной строки
    if (argc != 3) {
        printf("Использование: %s входной_файл выходной_файл\n", argv[0]);
        return 1;
    }
    
    // Проверка на одинаковые файлы (до открытия файлов)
    if (argv[1] != NULL && argv[2] != NULL && strcmp(argv[1], argv[2]) == 0) {
        printf("Ошибка: входной и выходной файлы не могут быть одинаковыми\n");
        return 1;
    }
    
    // Открываем входной файл
    FILE* input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        printf("Ошибка: не могу открыть файл %s\n", argv[1]);
        return 1;
    }
    
    // Открываем выходной файл
    FILE* output_file = fopen(argv[2], "w");
    if (output_file == NULL) {
        printf("Ошибка: не могу создать файл %s\n", argv[2]);
        fclose(input_file);
        return 1;
    }
    
    char buffer[MAX_LINE_LEN];
    char number[256];
    
    // Читаем файл по строкам
    while (fgets(buffer, sizeof(buffer), input_file) != NULL) {
        int i = 0;
        int len = strlen(buffer);
        
        // Обрабатываем каждый символ в строке
        while (i < len) {
            // Пропускаем пробелы, табуляции, переносы строк
            if (isspace(buffer[i])) {
                i++;
                continue;
            }
            
            // Нашли начало числа
            int j = 0;
            int has_sign = 0;
            
            // Обрабатываем знак в начале числа
            if (buffer[i] == '-' || buffer[i] == '+') {
                number[j++] = buffer[i++];
                has_sign = 1;
            }
            
            // Читаем цифры числа
            while (i < len && !isspace(buffer[i]) && 
                   (isalnum(buffer[i]) || buffer[i] == '-' || buffer[i] == '+')) {
                // Преобразуем в верхний регистр для единообразия
                char c = buffer[i];
                if (islower(c)) {
                    c = toupper(c);
                }
                number[j++] = c;
                i++;
            }
            number[j] = '\0';  // Завершаем строку
            
            // Если что-то считали (не только знак)
            if (j > has_sign) {
                // Удаляется нули (кроме случая "0")
                remove_leading_zeros(number);
                
                // Находим минимальное основание
                int min_base = find_min_base(number);
                
                // Конвертируем в десятичную систему
                long long decimal_value = convert_to_decimal(number, min_base);
                
                // Записываем в выходной файл
                fprintf(output_file, "%s %d %lld\n", number, min_base, decimal_value);
            }
            
            // Пропускаем разделители
            while (i < len && isspace(buffer[i])) {
                i++;
            }
        }
    }
    
    // Закрываем файлы
    fclose(input_file);
    fclose(output_file);
    
    printf("Обработка завершена. Результат в файле: %s\n", argv[2]);
    return 0;
}