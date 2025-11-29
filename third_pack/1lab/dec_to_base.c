#include "dec_to_base.h"
#include <stdio.h>

static const char DIGITS[] = "0123456789ABCDEFGHIJKLMNOPQRSTUV";

status_code dec_to_base_r(uint32_t number, uint32_t r, char* buffer, uint32_t buffer_size) {
    // Валидация входных параметров
    if (r == 0 || r > 5) return INVALID_BASE;
    if (buffer == NULL) return INVALID_NUMBER;
    
    // Основание системы счисления: 2^r
    uint32_t base = 1U << r;  
    uint32_t mask = base - 1; 
    
    // Вычисляем необходимое количество символов
    uint32_t temp = number;
    uint32_t digits_needed = 1;
    
    while (temp >= base) {
        temp >>= r;  // Эквивалентно делению на base
        digits_needed++;
    }
    
    // Проверяем размер буфера
    if (digits_needed + 1 > buffer_size) return BUFFER_TOO_SMALL;
    
    // Заполняем буфер с конца
    int32_t index = digits_needed - 1;
    buffer[digits_needed] = '\0';
    
    do {
        // Извлекаем группу из r битов
        uint32_t digit = number & mask;
        buffer[index--] = DIGITS[digit];
        number >>= r;  // Сдвигаем на r битов вправо
    } while (number != 0 && index >= 0);
    
    return SUCCESS;
}

void print_conversion(uint32_t number, uint32_t r) {
    char buffer[33]; // Максимальная длина для 32-битного числа
    
    status_code result = dec_to_base_r(number, r, buffer, sizeof(buffer));
    
    switch (result) {
        case SUCCESS:
            printf("Десятичное: %u -> Основание 2^%u: %s\n", number, r, buffer);
            break;
        case INVALID_BASE:
            printf("Ошибка: неверное основание %u (должно быть 1-5)\n", r);
            break;
        case INVALID_NUMBER:
            printf("Ошибка: неверное число или буфер\n");
            break;
        case BUFFER_TOO_SMALL:
            printf("Ошибка: буфер слишком мал\n");
            break;
    }
}
