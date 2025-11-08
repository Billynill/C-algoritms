#include "overscanf.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>

// a. %Ro - парсинг римских цифр
int parse_roman(const char *str, int *value) {
    if (str == NULL || value == NULL) return 0;
    
    const char *ptr = str;
    int result = 0;
    
    while (*ptr) {
        if (*ptr == 'M') {
            result += 1000;
            ptr++;
        } else if (strncmp(ptr, "CM", 2) == 0) {
            result += 900;
            ptr += 2;
        } else if (*ptr == 'D') {
            result += 500;
            ptr++;
        } else if (strncmp(ptr, "CD", 2) == 0) {
            result += 400;
            ptr += 2;
        } else if (*ptr == 'C') {
            if (ptr[1] == 'M') {
                result += 900;
                ptr += 2;
            } else if (ptr[1] == 'D') {
                result += 400;
                ptr += 2;
            } else {
                result += 100;
                ptr++;
            }
        } else if (*ptr == 'L') {
            result += 50;
            ptr++;
        } else if (strncmp(ptr, "XL", 2) == 0) {
            result += 40;
            ptr += 2;
        } else if (*ptr == 'X') {
            if (ptr[1] == 'C') {
                result += 90;
                ptr += 2;
            } else if (ptr[1] == 'L') {
                result += 40;
                ptr += 2;
            } else {
                result += 10;
                ptr++;
            }
        } else if (*ptr == 'V') {
            result += 5;
            ptr++;
        } else if (strncmp(ptr, "IV", 2) == 0) {
            result += 4;
            ptr += 2;
        } else if (*ptr == 'I') {
            if (ptr[1] == 'X') {
                result += 9;
                ptr += 2;
            } else if (ptr[1] == 'V') {
                result += 4;
                ptr += 2;
            } else {
                result += 1;
                ptr++;
            }
        } else {
            break;
        }
    }
    
    *value = result;
    return ptr - str;
}

// b. %Zr - парсинг цекендорфова представления
int parse_zeckendorf(const char *str, unsigned int *value) {
    if (str == NULL || value == NULL) return 0;
    
    const char *ptr = str;
    unsigned int result = 0;
    
    // Генерируем числа Фибоначчи
    unsigned int fib[50];
    int fib_count = 0;
    fib[0] = 1;
    fib[1] = 2;
    fib_count = 2;
    
    while (fib[fib_count - 1] <= 1000000) { // Достаточно для unsigned int
        fib[fib_count] = fib[fib_count - 1] + fib[fib_count - 2];
        fib_count++;
    }
    
    int bit_index = 0;
    while (*ptr && (*ptr == '0' || *ptr == '1')) {
        if (*ptr == '1') {
            if (bit_index < fib_count) {
                result += fib[bit_index];
            }
        }
        bit_index++;
        ptr++;
        
        // Проверяем завершающую 1
        if (*ptr == '1' && ptr[1] == '\0') {
            break;
        }
    }
    
    *value = result;
    return ptr - str;
}

// c. %Cv и d. %CV - парсинг из произвольной системы счисления
int parse_custom_base(const char *str, int *value, int base, int uppercase) {
    if (str == NULL || value == NULL) return 0;
    if (base < 2 || base > 36) base = 10;
    
    const char *ptr = str;
    long long result = 0;
    int is_negative = 0;
    int digits_parsed = 0;
    
    // Пропускаем пробелы
    while (*ptr == ' ') ptr++;
    
    // Обработка знака
    if (*ptr == '-') {
        is_negative = 1;
        ptr++;
    } else if (*ptr == '+') {
        ptr++;
    }
    
    // Парсинг цифр
    while (*ptr) {
        char c = *ptr;
        int digit = -1;
        
        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (c >= 'a' && c <= 'z' && !uppercase) {
            digit = c - 'a' + 10;
        } else if (c >= 'A' && c <= 'Z' && uppercase) {
            digit = c - 'A' + 10;
        } else if (c >= 'a' && c <= 'z' && uppercase) {
            // Для %CV пропускаем строчные буквы
            break;
        } else if (c >= 'A' && c <= 'Z' && !uppercase) {
            // Для %Cv пропускаем заглавные буквы
            break;
        } else {
            break;
        }
        
        if (digit >= base) break;
        
        result = result * base + digit;
        digits_parsed++;
        ptr++;
    }
    
    if (digits_parsed == 0) return 0;
    
    if (is_negative) {
        result = -result;
    }
    
    *value = (int)result;
    return ptr - str;
}

// Основная функция сканирования
static int overscanf_core(const char *str, FILE *stream, const char *format, va_list args) {
    const char *fmt_ptr = format;
    const char *str_ptr = str;
    int assignments = 0;
    
    while (*fmt_ptr) {
        if (*fmt_ptr != '%') {
            if (stream) {
                // Для файлового ввода - читаем и проверяем символ
                int c = fgetc(stream);
                if (c == EOF || c != *fmt_ptr) {
                    if (c != EOF) ungetc(c, stream);
                    break;
                }
            } else {
                // Для строкового ввода - проверяем символ
                if (*str_ptr != *fmt_ptr) break;
                str_ptr++;
            }
            fmt_ptr++;
            continue;
        }
        
        fmt_ptr++; // Пропускаем '%'
        
        // Обработка специальных флагов
        if (fmt_ptr[0] == 'R' && fmt_ptr[1] == 'o') { // %Ro
            int *value_ptr = va_arg(args, int*);
            int chars_parsed = 0;
            
            if (stream) {
                // Читаем из файла
                char buffer[100];
                int i = 0;
                int c;
                while ((c = fgetc(stream)) != EOF && isalpha(c) && i < 99) {
                    buffer[i++] = c;
                }
                if (c != EOF) ungetc(c, stream);
                buffer[i] = '\0';
                chars_parsed = parse_roman(buffer, value_ptr);
            } else {
                // Читаем из строки
                chars_parsed = parse_roman(str_ptr, value_ptr);
                str_ptr += chars_parsed;
            }
            
            if (chars_parsed > 0) assignments++;
            fmt_ptr += 2;
            
        } else if (fmt_ptr[0] == 'Z' && fmt_ptr[1] == 'r') { // %Zr
            unsigned int *value_ptr = va_arg(args, unsigned int*);
            int chars_parsed = 0;
            
            if (stream) {
                // Читаем из файла
                char buffer[100];
                int i = 0;
                int c;
                while ((c = fgetc(stream)) != EOF && (c == '0' || c == '1') && i < 99) {
                    buffer[i++] = c;
                }
                if (c != EOF) ungetc(c, stream);
                buffer[i] = '\0';
                chars_parsed = parse_zeckendorf(buffer, value_ptr);
            } else {
                // Читаем из строки
                chars_parsed = parse_zeckendorf(str_ptr, value_ptr);
                str_ptr += chars_parsed;
            }
            
            if (chars_parsed > 0) assignments++;
            fmt_ptr += 2;
            
        } else if (fmt_ptr[0] == 'C' && (fmt_ptr[1] == 'v' || fmt_ptr[1] == 'V')) { // %Cv или %CV
            int *value_ptr = va_arg(args, int*);
            int base = va_arg(args, int);
            int uppercase = (fmt_ptr[1] == 'V');
            int chars_parsed = 0;
            
            if (stream) {
                // Читаем из файла
                char buffer[100];
                int i = 0;
                int c;
                while ((c = fgetc(stream)) != EOF && (isdigit(c) || 
                       (uppercase && isupper(c)) || (!uppercase && islower(c)) ||
                       c == '+' || c == '-') && i < 99) {
                    buffer[i++] = c;
                }
                if (c != EOF) ungetc(c, stream);
                buffer[i] = '\0';
                chars_parsed = parse_custom_base(buffer, value_ptr, base, uppercase);
            } else {
                // Читаем из строки
                chars_parsed = parse_custom_base(str_ptr, value_ptr, base, uppercase);
                str_ptr += chars_parsed;
            }
            
            if (chars_parsed > 0) assignments++;
            fmt_ptr += 2;
            
        } else {
            // Стандартные спецификаторы - упрощенная реализация
            if (*fmt_ptr == 'd') {
                int *value_ptr = va_arg(args, int*);
                if (stream) {
                    if (fscanf(stream, "%d", value_ptr) == 1) assignments++;
                } else {
                    if (sscanf(str_ptr, "%d", value_ptr) == 1) {
                        assignments++;
                        // Пропускаем прочитанное число в строке
                        while (*str_ptr && !isspace(*str_ptr)) str_ptr++;
                    }
                }
                fmt_ptr++;
            } else if (*fmt_ptr == 'u') {
                unsigned int *value_ptr = va_arg(args, unsigned int*);
                if (stream) {
                    if (fscanf(stream, "%u", value_ptr) == 1) assignments++;
                } else {
                    if (sscanf(str_ptr, "%u", value_ptr) == 1) {
                        assignments++;
                        while (*str_ptr && !isspace(*str_ptr)) str_ptr++;
                    }
                }
                fmt_ptr++;
            } else if (*fmt_ptr == 's') {
                char *str_dest = va_arg(args, char*);
                if (stream) {
                    if (fscanf(stream, "%s", str_dest) == 1) assignments++;
                } else {
                    // Пропускаем пробелы
                    while (*str_ptr == ' ') str_ptr++;
                    // Копируем слово
                    int i = 0;
                    while (*str_ptr && !isspace(*str_ptr)) {
                        str_dest[i++] = *str_ptr++;
                    }
                    str_dest[i] = '\0';
                    if (i > 0) assignments++;
                }
                fmt_ptr++;
            } else {
                // Неизвестный спецификатор - пропускаем
                fmt_ptr++;
            }
        }
    }
    
    return assignments;
}

// Реализация функций
int overfscanf(FILE *stream, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = overscanf_core(NULL, stream, format, args);
    va_end(args);
    return result;
}

int oversscanf(const char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = overscanf_core(str, NULL, format, args);
    va_end(args);
    return result;
}