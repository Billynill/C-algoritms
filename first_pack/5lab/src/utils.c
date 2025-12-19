#include "utils.h"
#include <ctype.h>

// Проверяем является ли символ латинской буквой (A-Z, a-z)
bool is_latin_letter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

// Проверяем является ли символ арабской цифрой (0-9)
bool is_arabic_digit(char c) {
    return c >= '0' && c <= '9';
}

// Проверяем является ли символ пробельным (пробел, таб, перенос строки)
bool is_whitespace(char c) {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

// Проверяем является ли символ специальным
// Специальный = не буква, не цифра, не пробел
bool is_special_char(char c) {
    // Если это пробельный символ - не специальный
    if (is_whitespace(c)) {
        return false;
    }
    
    // Если это буква - не специальный
    if (is_latin_letter(c)) {
        return false;
    }
    
    // Если это цифра - не специальный
    if (is_arabic_digit(c)) {
        return false;
    }
    
    // Всё остальное - специальный символ
    return true;
}
