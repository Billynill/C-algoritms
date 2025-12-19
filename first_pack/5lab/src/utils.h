#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

// Проверка является ли символ латинской буквой
bool is_latin_letter(char c);

// Проверка является ли символ арабской цифрой
bool is_arabic_digit(char c);

// Проверка является ли символ специальным (не буква, не цифра, не пробел)
bool is_special_char(char c);

#endif
