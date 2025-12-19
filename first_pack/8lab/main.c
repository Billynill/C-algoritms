#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Прототипы функций (объявления)
int char_to_digit(char c);
char digit_to_char(int d);
int check_number(char* s, int base);
long long to_number(char* s, int base);
void to_string(long long n, int base, char* result);
void trim_zeros(char* s);

#define MAX_LEN 256

int main() {
    int base;
    char input[MAX_LEN];
    
    // Ввод основания
    printf("Основание (2-36): ");
    fgets(input, MAX_LEN, stdin);
    input[strcspn(input, "\n")] = '\0';
    
    base = atoi(input);
    if (base < 2 || base > 36) {
        printf("Ошибка: основание должно быть от 2 до 36\n");
        return 1;
    }
    
    printf("Вводите числа (Stop - закончить):\n");
    
    long long nums[1000];
    char strings[1000][MAX_LEN];
    int count = 0;
    
    // Чтение чисел
    while (1) {
        fgets(input, MAX_LEN, stdin);
        input[strcspn(input, "\n")] = '\0';
        
        if (count >= 1000) {
            printf("Слишком много чисел!\n");
            break;
        }
        
        // Сначала проверяем, является ли строка валидным числом
        // Только если это не число, проверяем на команду "Stop"
        if (!check_number(input, base)) {
            // Проверяем команду "Stop" только если это не валидное число
            if (strcmp(input, "Stop") == 0 || strcmp(input, "stop") == 0 || 
                strcmp(input, "STOP") == 0) {
                break;
            }
            printf("Неверное число: %s\n", input);
            continue;
        }
        
        nums[count] = to_number(input, base);
        strcpy(strings[count], input);
        trim_zeros(strings[count]);
        count++;
    }
    
    if (count == 0) {
        printf("Нет чисел\n");
        return 0;
    }
    
    // Поиск максимального по модулю
    long long max_num = nums[0];
    long long max_abs = (max_num < 0) ? -max_num : max_num;
    char* max_str = strings[0];
    
    for (int i = 1; i < count; i++) {
        long long abs_val = (nums[i] < 0) ? -nums[i] : nums[i];
        if (abs_val > max_abs) {
            max_abs = abs_val;
            max_num = nums[i];
            max_str = strings[i];
        }
    }
    
    // Вывод результатов
    printf("\nМаксимальное по модулю: %s\n", max_str);
    
    char buf[MAX_LEN];
    int bases[] = {9, 18, 27, 36};
    
    for (int i = 0; i < 4; i++) {
        to_string(max_num, bases[i], buf);
        printf("В %d-ричной: %s\n", bases[i], buf);
    }
    
    return 0;
}