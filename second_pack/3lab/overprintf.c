#include "overprintf.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

// Вспомогательная функция для преобразования в строку в заданной системе счисления
static int to_base(char *buffer, long long value, int base, int uppercase) {
    if (base < 2 || base > 36) return 0;
    
    const char *digits = uppercase ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" 
                                   : "0123456789abcdefghijklmnopqrstuvwxyz";
    
    char temp[65];
    int i = 0;
    int is_negative = 0;
    
    if (value < 0) {
        is_negative = 1;
        value = -value;
    }
    
    if (value == 0) {
        temp[i++] = '0';
    } else {
        while (value > 0) {
            temp[i++] = digits[value % base];
            value /= base;
        }
    }
    
    int j = 0;
    if (is_negative) {
        buffer[j++] = '-';
    }
    
    while (i > 0) {
        buffer[j++] = temp[--i];
    }
    
    buffer[j] = '\0';
    return j;
}

// a. %Ro - римские цифры
int handle_roman(char *buffer, int value) {
    if (value <= 0 || value > 3999) {
        return sprintf(buffer, "%d", value);
    }
    
    const char *thousands[] = {"", "M", "MM", "MMM"};
    const char *hundreds[] = {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"};
    const char *tens[] = {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"};
    const char *ones[] = {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"};
    
    char temp[20];
    int pos = 0;
    
    pos += sprintf(temp + pos, "%s", thousands[value / 1000]);
    pos += sprintf(temp + pos, "%s", hundreds[(value % 1000) / 100]);
    pos += sprintf(temp + pos, "%s", tens[(value % 100) / 10]);
    pos += sprintf(temp + pos, "%s", ones[value % 10]);
    
    strcpy(buffer, temp);
    return pos;
}

// b. %Zr - цекендорфово представление (числа Фибоначчи)
int handle_zeckendorf(char *buffer, unsigned int value) {
    if (value == 0) {
        strcpy(buffer, "0");
        return 1;
    }
    
    // Находим числа Фибоначчи до value
    unsigned int fib[50];
    int fib_count = 0;
    fib[0] = 1;
    fib[1] = 2;
    fib_count = 2;
    
    while (fib[fib_count - 1] <= value) {
        fib[fib_count] = fib[fib_count - 1] + fib[fib_count - 2];
        fib_count++;
    }
    
    char temp[50] = {0};
    unsigned int n = value;
    
    // Строим представление
    for (int i = fib_count - 1; i >= 0; i--) {
        if (n >= fib[i]) {
            strcat(temp, "1");
            n -= fib[i];
        } else if (strlen(temp) > 0) {
            strcat(temp, "0");
        }
    }
    
    // Добавляем завершающую 1
    strcat(temp, "1");
    strcpy(buffer, temp);
    return strlen(temp);
}

// c. %Cv и d. %CV - система счисления с основанием
int handle_custom_base(char *buffer, int value, int base, int uppercase) {
    if (base < 2 || base > 36) {
        base = 10;
    }
    return to_base(buffer, value, base, uppercase);
}

// e. %to и f. %TO - из произвольной системы в десятичную
int handle_to_decimal(char *buffer, const char *str, int base, int uppercase) {
    if (base < 2 || base > 36) {
        base = 10;
    }
    
    if (str == NULL || *str == '\0') {
        return sprintf(buffer, "0");
    }
    
    long long result = 0;
    const char *ptr = str;
    int is_negative = 0;
    
    // Обработка знака
    if (*ptr == '-') {
        is_negative = 1;
        ptr++;
    }
    
    // Преобразование
    while (*ptr) {
        char c = *ptr;
        int digit;
        
        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (c >= 'a' && c <= 'z') {
            digit = c - 'a' + 10;
        } else if (c >= 'A' && c <= 'Z') {
            digit = c - 'A' + 10;
        } else {
            break;
        }
        
        if (digit >= base) break;
        
        result = result * base + digit;
        ptr++;
    }
    
    if (is_negative) {
        result = -result;
    }
    
    return sprintf(buffer, "%lld", result);
}

// Вспомогательная функция для дампа памяти
static int byte_to_binary(char *buffer, unsigned char byte) {
    int pos = 0;
    for (int i = 7; i >= 0; i--) {
        buffer[pos++] = (byte & (1 << i)) ? '1' : '0';
    }
    buffer[pos] = '\0';
    return pos;
}

// g. %mi - дамп памяти int
int handle_memory_dump_int(char *buffer, int value) {
    unsigned char *bytes = (unsigned char *)&value;
    int pos = 0;
    
    for (int i = 0; i < 4; i++) {
        pos += byte_to_binary(buffer + pos, bytes[i]);
        if (i < 3) {
            buffer[pos++] = ' ';
        }
    }
    buffer[pos] = '\0';
    return pos;
}

// h. %mu - дамп памяти unsigned int
int handle_memory_dump_uint(char *buffer, unsigned int value) {
    return handle_memory_dump_int(buffer, (int)value);
}

// i. %md - дамп памяти double
int handle_memory_dump_double(char *buffer, double value) {
    unsigned char *bytes = (unsigned char *)&value;
    int pos = 0;
    
    for (int i = 0; i < 8; i++) {
        pos += byte_to_binary(buffer + pos, bytes[i]);
        if (i < 7) {
            buffer[pos++] = ' ';
        }
    }
    buffer[pos] = '\0';
    return pos;
}

// j. %mf - дамп памяти float
int handle_memory_dump_float(char *buffer, float value) {
    unsigned char *bytes = (unsigned char *)&value;
    int pos = 0;
    
    for (int i = 0; i < 4; i++) {
        pos += byte_to_binary(buffer + pos, bytes[i]);
        if (i < 3) {
            buffer[pos++] = ' ';
        }
    }
    buffer[pos] = '\0';
    return pos;
}

// Основная функция форматирования
static int overprintf_core(char *str, FILE *stream, const char *format, va_list args) {
    char buffer[1024];
    int total_chars = 0;
    const char *ptr = format;
    
    while (*ptr) {
        if (*ptr != '%') {
            if (str) {
                *str++ = *ptr;
            } else if (stream) {
                fputc(*ptr, stream);
            }
            total_chars++;
            ptr++;
            continue;
        }
        
        ptr++; 
        
        // Обработка специальных флагов
        if (ptr[0] == 'R' && ptr[1] == 'o') { // %Ro
            int value = va_arg(args, int);
            int len = handle_roman(buffer, value);
            if (str) {
                strcpy(str, buffer);
                str += len;
            } else if (stream) {
                fwrite(buffer, 1, len, stream);
            }
            total_chars += len;
            ptr += 2;
            
        } else if (ptr[0] == 'Z' && ptr[1] == 'r') { // %Zr
            unsigned int value = va_arg(args, unsigned int);
            int len = handle_zeckendorf(buffer, value);
            if (str) {
                strcpy(str, buffer);
                str += len;
            } else if (stream) {
                fwrite(buffer, 1, len, stream);
            }
            total_chars += len;
            ptr += 2;
            
        } else if (ptr[0] == 'C' && (ptr[1] == 'v' || ptr[1] == 'V')) { // %Cv или %CV
            int value = va_arg(args, int);
            int base = va_arg(args, int);
            int uppercase = (ptr[1] == 'V');
            int len = handle_custom_base(buffer, value, base, uppercase);
            if (str) {
                strcpy(str, buffer);
                str += len;
            } else if (stream) {
                fwrite(buffer, 1, len, stream);
            }
            total_chars += len;
            ptr += 2;
            
        } else if (ptr[0] == 't' && ptr[1] == 'o') { // %to
            char *input_str = va_arg(args, char*);
            int base = va_arg(args, int);
            int len = handle_to_decimal(buffer, input_str, base, 0);
            if (str) {
                strcpy(str, buffer);
                str += len;
            } else if (stream) {
                fwrite(buffer, 1, len, stream);
            }
            total_chars += len;
            ptr += 2;
            
        } else if (ptr[0] == 'T' && ptr[1] == 'O') { // %TO
            char *input_str = va_arg(args, char*);
            int base = va_arg(args, int);
            int len = handle_to_decimal(buffer, input_str, base, 1);
            if (str) {
                strcpy(str, buffer);
                str += len;
            } else if (stream) {
                fwrite(buffer, 1, len, stream);
            }
            total_chars += len;
            ptr += 2;
            
        } else if (ptr[0] == 'm' && ptr[1] == 'i') { // %mi
            int value = va_arg(args, int);
            int len = handle_memory_dump_int(buffer, value);
            if (str) {
                strcpy(str, buffer);
                str += len;
            } else if (stream) {
                fwrite(buffer, 1, len, stream);
            }
            total_chars += len;
            ptr += 2;
            
        } else if (ptr[0] == 'm' && ptr[1] == 'u') { // %mu
            unsigned int value = va_arg(args, unsigned int);
            int len = handle_memory_dump_uint(buffer, value);
            if (str) {
                strcpy(str, buffer);
                str += len;
            } else if (stream) {
                fwrite(buffer, 1, len, stream);
            }
            total_chars += len;
            ptr += 2;
            
        } else if (ptr[0] == 'm' && ptr[1] == 'd') { // %md
            double value = va_arg(args, double);
            int len = handle_memory_dump_double(buffer, value);
            if (str) {
                strcpy(str, buffer);
                str += len;
            } else if (stream) {
                fwrite(buffer, 1, len, stream);
            }
            total_chars += len;
            ptr += 2;
            
        } else if (ptr[0] == 'm' && ptr[1] == 'f') { // %mf
            float value = va_arg(args, double); 
            int len = handle_memory_dump_float(buffer, value);
            if (str) {
                strcpy(str, buffer);
                str += len;
            } else if (stream) {
                fwrite(buffer, 1, len, stream);
            }
            total_chars += len;
            ptr += 2;
            
        } else {
            if (str) {
                *str++ = '%';
                if (*ptr) *str++ = *ptr;
            } else if (stream) {
                fputc('%', stream);
                if (*ptr) fputc(*ptr, stream);
            }
            total_chars += 1 + (*ptr ? 1 : 0);
            if (*ptr) ptr++;
        }
    }
    
    if (str) {
        *str = '\0';
    }
    
    return total_chars;
}

int overfprintf(FILE *stream, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = overprintf_core(NULL, stream, format, args);
    va_end(args);
    return result;
}

int oversprintf(char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = overprintf_core(str, NULL, format, args);
    va_end(args);
    return result;
}