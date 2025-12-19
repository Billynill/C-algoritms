#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Преобразование символа в цифру
int char_to_digit(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
    if (c >= 'a' && c <= 'z') return c - 'a' + 10;
    return -1;
}

// Преобразование цифры в символ
char digit_to_char(int d) {
    if (d >= 0 && d <= 9) return '0' + d;
    if (d >= 10 && d <= 35) return 'A' + (d - 10);
    return '?';
}

// Проверка, является ли строка числом в заданной системе
int check_number(char* s, int base) {
    int i = 0;
    
    if (s == NULL || s[0] == '\0') return 0;
    
    // Пропускаем знак
    if (s[i] == '-' || s[i] == '+') i++;
    
    // Проверяем все символы
    while (s[i]) {
        int d = char_to_digit(s[i]);
        if (d == -1 || d >= base) return 0;
        i++;
    }
    
    return 1;
}

// Преобразование строки в число
long long to_number(char* s, int base) {
    long long n = 0;
    int i = 0, sign = 1;
    
    if (s == NULL || s[0] == '\0') return 0;
    
    // Знак
    if (s[i] == '-') { sign = -1; i++; }
    else if (s[i] == '+') i++;
    
    // Пропуск нулей
    while (s[i] == '0' && s[i+1]) i++;
    
    // Основное преобразование
    while (s[i]) {
        n = n * base + char_to_digit(s[i]);
        i++;
    }
    
    return n * sign;
}

// Преобразование числа в строку
void to_string(long long n, int base, char* result) {
    char temp[65];
    int i = 0, neg = 0;
    
    if (n == 0) {
        strcpy(result, "0");
        return;
    }
    
    if (n < 0) {
        neg = 1;
        n = -n;
    }
    
    // Записываем в обратном порядке
    while (n > 0) {
        temp[i++] = digit_to_char(n % base);
        n /= base;
    }
    
    if (neg) temp[i++] = '-';
    
    // Разворачиваем
    int k = 0;
    for (int j = i-1; j >= 0; j--) {
        result[k++] = temp[j];
    }
    result[k] = '\0';
}

// Удаление ведущих нулей
void trim_zeros(char* s) {
    int start = 0;
    
    if (s == NULL || s[0] == '\0') return;
    
    // Пропускаем знак
    if (s[0] == '-' || s[0] == '+') start = 1;
    
    // Находим первую ненулевую цифру
    int i = start;
    while (s[i] == '0' && s[i+1]) i++;
    
    // Если все нули
    if (s[i] == '\0') {
        if (start) { s[1] = '0'; s[2] = '\0'; }
        else { s[0] = '0'; s[1] = '\0'; }
        return;
    }
    
    // Сдвигаем если нужно
    if (i > start) {
        int len = strlen(s + i);
        if (start) {
            // Со знаком
            for (int j = 0; j <= len; j++) {
                s[start + j] = s[i + j];
            }
        } else {
            // Без знака
            for (int j = 0; j <= len; j++) {
                s[j] = s[i + j];
            }
        }
    }
}