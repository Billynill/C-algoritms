#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//СЛОЖНОСТЬ АЛГОРИТМА O(N × L)

// Преобразование символа в цифру
int char_to_digit(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 10;
    }
    return -1;  // Неверный символ
}

// Преобразование цифры в символ
char digit_to_char(int d) {
    if (d >= 0 && d <= 9) {
        return '0' + d;
    } else if (d >= 10 && d <= 35) {
        return 'A' + (d - 10);
    }
    return '?';
}

// Функция для нахождения минимального основания системы счисления
int find_min_base(const char* num) {
    if (num == NULL || num[0] == '\0') {
        return 2;  // Минимальное возможное
    }
    
    int max_digit = 1;  // Минимум основание 2 (цифры 0 и 1)
    int i = 0;
    
    // Пропускаем знак
    if (num[i] == '-' || num[i] == '+') {
        i++;
    }
    
    // Если только "0" или "-0" или "+0"
    if (num[i] == '0' && num[i+1] == '\0') {
        return 2;  // Минимальное основание
    }
    
    // Ищем максимальную цифру в числе
    while (num[i] != '\0') {
        int digit = char_to_digit(num[i]);
        if (digit > max_digit) {
            max_digit = digit;
        }
        i++;
    }
    
    // Основание = максимальная цифра + 1
    // Но не меньше 2
    int base = max_digit + 1;
    if (base < 2) {
        base = 2;
    }
    
    return base;
}

// Конвертация числа в десятичную систему
long long convert_to_decimal(const char* num, int base) {
    if (num == NULL || num[0] == '\0') {
        return 0;
    }
    
    long long result = 0;
    int i = 0;
    int sign = 1;
    
    // Обработка знака
    if (num[i] == '-') {
        sign = -1;
        i++;
    } else if (num[i] == '+') {
        i++;
    }
    
    // Пропускаем ведущие нули
    while (num[i] == '0' && num[i+1] != '\0') {
        i++;
    }
    
    // Конвертация
    while (num[i] != '\0') {
        int digit = char_to_digit(num[i]);
        result = result * base + digit;
        i++;
    }
    
    return result * sign;
}

// Удаление ведущих нулей
void remove_leading_zeros(char* str) {
    if (str == NULL || str[0] == '\0') {
        return;
    }
    
    int has_sign = 0;
    int start = 0;
    
    // Проверяем знак
    if (str[0] == '-' || str[0] == '+') {
        has_sign = 1;
        start = 1;
    }
    
    // Находим первую ненулевую цифру
    int i = start;
    while (str[i] == '0' && str[i+1] != '\0') {
        i++;
    }
    
    // Если все нули - оставляем один ноль
    if (str[i] == '\0') {
        if (has_sign) {
            str[1] = '0';
            str[2] = '\0';
        } else {
            str[0] = '0';
            str[1] = '\0';
        }
        return;
    }
    
    // Сдвигаем если нужно
    if (i > start) {
        int len = strlen(str + i);
        if (has_sign) {
            // Со знаком: знак + цифры
            str[1] = str[i];  // Первая цифра
            for (int j = 1; j <= len; j++) {
                str[1 + j] = str[i + j];
            }
        } else {
            // Без знака
            for (int j = 0; j <= len; j++) {
                str[j] = str[i + j];
            }
        }
    }
}

// Проверка, является ли символ допустимым для числа
int is_valid_number_char(char c) {
    return isalnum(c) || c == '-' || c == '+';
}

// Удаление пробелов в начале и конце строки
void trim(char* str) {
    if (str == NULL) return;
    
    int len = strlen(str);
    int start = 0, end = len - 1;
    
    // Находим первый не пробельный символ
    while (start < len && isspace(str[start])) {
        start++;
    }
    
    // Находим последний не пробельный символ
    while (end >= 0 && isspace(str[end])) {
        end--;
    }
    
    // Сдвигаем строку
    if (start > 0) {
        for (int i = 0; i <= end - start + 1; i++) {
            str[i] = str[start + i];
        }
    }
    
    // Обрезаем конец
    str[end - start + 1] = '\0';
}