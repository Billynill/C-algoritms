#include <stdio.h>
#include "string_functions.h"

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void test_memchr(void) {
    printf("\n=== memchr ===\n");
    char str[100];
    char ch;
    int n;
    
    printf("Введите строку: ");
    scanf("%99s", str);
    clear_input_buffer();
    
    printf("Введите символ для поиска: ");
    scanf("%c", &ch);
    clear_input_buffer();
    
    printf("Введите количество байт для поиска: ");
    scanf("%d", &n);
    clear_input_buffer();
    
    char *result = memchr(str, ch, n);
    if (result) {
        printf("Символ '%c' найден в позиции: %ld\n", ch, result - str);
    } else {
        printf("Символ '%c' не найден в первых %d байтах\n", ch, n);
    }
}

void test_memcmp(void) {
    printf("\n=== memcmp ===\n");
    char str1[100], str2[100];
    int n;
    
    printf("Введите первую строку: ");
    scanf("%99s", str1);
    clear_input_buffer();
    
    printf("Введите вторую строку: ");
    scanf("%99s", str2);
    clear_input_buffer();
    
    printf("Введите количество байт для сравнения: ");
    scanf("%d", &n);
    clear_input_buffer();
    
    int result = memcmp(str1, str2, n);
    printf("Результат сравнения: %d\n", result);
}

void test_memcpy(void) {
    printf("\n=== memcpy ===\n");
    char src[100], dest[100];
    int n;
    
    printf("Введите строку-источник: ");
    scanf("%99s", src);
    clear_input_buffer();
    
    printf("Введите количество байт для копирования: ");
    scanf("%d", &n);
    clear_input_buffer();
    
    // Показываем пустой destination до копирования
    printf("Destination до копирования: (пусто)\n");
    memcpy(dest, src, n);
    dest[n] = '\0';
    printf("Результат копирования: %s\n", dest);
}

void test_memset(void) {
    printf("\n=== memset ===\n");
    char str[100];
    char ch;
    int n;
    
    printf("Введите строку для заполнения: ");
    scanf("%99s", str);
    clear_input_buffer();
    
    printf("Исходная строка: %s\n", str);
    printf("Введите символ для заполнения: ");
    scanf(" %c", &ch);
    clear_input_buffer();
    
    printf("Введите количество байт для заполнения: ");
    scanf("%d", &n);
    clear_input_buffer();
    
    memset(str, ch, n);
    str[n] = '\0';
    printf("Результат заполнения: %s\n", str);
}

void test_strncat(void) {
    printf("\n=== strncat ===\n");
    char dest[200], src[100];
    int n;
    
    printf("Введите начальную строку (dest): ");
    scanf("%99s", dest);
    clear_input_buffer();
    
    printf("Введите строку для добавления (src): ");
    scanf("%99s", src);
    clear_input_buffer();
    
    printf("Введите количество символов для добавления: ");
    scanf("%d", &n);
    clear_input_buffer();
    
    printf("Dest до конкатенации: %s\n", dest);
    strncat(dest, src, n);
    printf("Результат конкатенации: %s\n", dest);
}

void test_strchr(void) {
    printf("\n=== strchr ===\n");
    char str[100];
    char ch;
    
    printf("Введите строку: ");
    scanf("%99s", str);
    clear_input_buffer();
    
    printf("Введите символ для поиска: ");
    scanf("%c", &ch);
    clear_input_buffer();
    
    char *result = strchr(str, ch);
    if (result) {
        printf("Символ '%c' найден в позиции: %ld\n", ch, result - str);
    } else {
        printf("Символ '%c' не найден\n", ch);
    }
}

void test_strncmp(void) {
    printf("\n=== strncmp ===\n");
    char str1[100], str2[100];
    int n;
    
    printf("Введите первую строку: ");
    scanf("%99s", str1);
    clear_input_buffer();
    
    printf("Введите вторую строку: ");
    scanf("%99s", str2);
    clear_input_buffer();
    
    printf("Введите количество символов для сравнения: ");
    scanf("%d", &n);
    clear_input_buffer();
    
    int result = strncmp(str1, str2, n);
    printf("Результат сравнения: %d\n", result);
}

void test_strncpy(void) {
    printf("\n=== strncpy ===\n");
    char src[100], dest[100];
    int n;
    
    printf("Введите строку-источник: ");
    scanf("%99s", src);
    clear_input_buffer();
    
    printf("Введите количество символов для копирования: ");
    scanf("%d", &n);
    clear_input_buffer();
    
    printf("Destination до копирования: (пусто)\n");
    strncpy(dest, src, n);
    dest[n] = '\0';
    printf("Результат копирования: %s\n", dest);
}

void test_strcspn(void) {
    printf("\n=== strcspn ===\n");
    char str1[100], str2[100];
    
    printf("Введите основную строку: ");
    scanf("%99s", str1);
    clear_input_buffer();
    
    printf("Введите строку с запрещенными символами: ");
    scanf("%99s", str2);
    clear_input_buffer();
    
    size_t result = strcspn(str1, str2);
    printf("Длина сегмента без запрещенных символов: %zu\n", result);
}

void test_strerror(void) {
    printf("\n=== strerror ===\n");
    int errnum;
    
    printf("Введите номер ошибки (0-9): ");
    scanf("%d", &errnum);
    clear_input_buffer();
    
    char *result = strerror(errnum);
    printf("Описание ошибки %d: %s\n", errnum, result);
}

void test_strlen(void) {
    printf("\n=== strlen ===\n");
    char str[100];
    
    printf("Введите строку: ");
    scanf("%99s", str);
    clear_input_buffer();
    
    size_t result = strlen(str);
    printf("Длина строки: %zu\n", result);
}

void test_strpbrk(void) {
    printf("\n=== strpbrk ===\n");
    char str1[100], str2[100];
    
    printf("Введите основную строку: ");
    scanf("%99s", str1);
    clear_input_buffer();
    
    printf("Введите строку с искомыми символами: ");
    scanf("%99s", str2);
    clear_input_buffer();
    
    char *result = strpbrk(str1, str2);
    if (result) {
        printf("Найден символ '%c' в позиции: %ld\n", *result, result - str1);
    } else {
        printf("Ни один из символов не найден\n");
    }
}

void test_strrchr(void) {
    printf("\n=== strrchr ===\n");
    char str[100];
    char ch;
    
    printf("Введите строку: ");
    scanf("%99s", str);
    clear_input_buffer();
    
    printf("Введите символ для поиска: ");
    scanf("%c", &ch);
    clear_input_buffer();
    
    char *result = strrchr(str, ch);
    if (result) {
        printf("Последний символ '%c' найден в позиции: %ld\n", ch, result - str);
    } else {
        printf("Символ '%c' не найден\n", ch);
    }
}

void test_strstr(void) {
    printf("\n=== strstr ===\n");
    char haystack[100], needle[100];
    
    printf("Введите строку для поиска: ");
    scanf("%99s", haystack);
    clear_input_buffer();
    
    printf("Введите подстроку для поиска: ");
    scanf("%99s", needle);
    clear_input_buffer();
    
    char *result = strstr(haystack, needle);
    if (result) {
        printf("Подстрока найдена в позиции: %ld\n", result - haystack);
    } else {
        printf("Подстрока не найдена\n");
    }
}

void test_strtok(void) {
    printf("\n=== strtok ===\n");
    char str[200];
    char delim[10];
    
    printf("Введите строку для разбиения: ");
    scanf(" %[^\n]", str);
    clear_input_buffer();
    
    printf("Введите разделители: ");
    scanf("%9s", delim);
    clear_input_buffer();
    
    printf("Токены:\n");
    char *token = strtok(str, delim);
    int count = 1;
    while (token != NULL) {
        printf("  %d. %s\n", count++, token);
        token = strtok(NULL, delim);
    }
}

int main(void) {
    int choice;
    
    do {
        printf("\n=== ТЕСТИРОВАНИЕ СТРОКОВЫХ ФУНКЦИЙ ===\n");
        printf("1. memchr   - поиск символа в памяти\n");
        printf("2. memcmp   - сравнение памяти\n");
        printf("3. memcpy   - копирование памяти\n");
        printf("4. memset   - заполнение памяти\n");
        printf("5. strncat  - конкатенация строк\n");
        printf("6. strchr   - поиск первого вхождения символа\n");
        printf("7. strncmp  - сравнение строк\n");
        printf("8. strncpy  - копирование строк\n");
        printf("9. strcspn  - длина сегмента без символов\n");
        printf("10. strerror - описание ошибки\n");
        printf("11. strlen   - длина строки\n");
        printf("12. strpbrk  - поиск любого символа из набора\n");
        printf("13. strrchr  - поиск последнего вхождения\n");
        printf("14. strstr   - поиск подстроки\n");
        printf("15. strtok   - разбиение на токены\n");
        printf("0. Выход\n");
        printf("Выберите функцию для тестирования: ");
        
        scanf("%d", &choice);
        clear_input_buffer();
        
        switch (choice) {
            case 1: test_memchr(); break;
            case 2: test_memcmp(); break;
            case 3: test_memcpy(); break;
            case 4: test_memset(); break;
            case 5: test_strncat(); break;
            case 6: test_strchr(); break;
            case 7: test_strncmp(); break;
            case 8: test_strncpy(); break;
            case 9: test_strcspn(); break;
            case 10: test_strerror(); break;
            case 11: test_strlen(); break;
            case 12: test_strpbrk(); break;
            case 13: test_strrchr(); break;
            case 14: test_strstr(); break;
            case 15: test_strtok(); break;
            case 0: printf("Выход...\n"); break;
            default: printf("Неверный выбор!\n");
        }
        
        if (choice != 0) {
            printf("\nНажмите Enter для продолжения...");
            getchar();
        }
        
    } while (choice != 0);
    
    return 0;
}
