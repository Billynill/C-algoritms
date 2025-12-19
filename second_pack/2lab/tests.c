#include <stdio.h>
#include <stdlib.h>
#include "string_functions.h"

// Вспомогательная функция для сравнения строк (используем нашу реализацию)
static int my_strcmp(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) return 0;
    while (*s1 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

// Счетчики для тестов
static int tests_passed = 0;
static int tests_failed = 0;

// Макрос для проверки условия
#define ASSERT(condition, message) \
    do { \
        if (condition) { \
            printf("PASS: %s\n", message); \
            tests_passed++; \
        } else { \
            printf("FAIL: %s\n", message); \
            tests_failed++; \
        } \
    } while (0)

// Макрос для проверки равенства
#define ASSERT_EQ(a, b, message) \
    ASSERT((a) == (b), message)

// Макрос для проверки указателей
#define ASSERT_PTR_EQ(a, b, message) \
    ASSERT((a) == (b), message)

// Макрос для проверки что указатель не NULL
#define ASSERT_NOT_NULL(ptr, message) \
    ASSERT((ptr) != NULL, message)

// Макрос для проверки что указатель NULL
#define ASSERT_NULL(ptr, message) \
    ASSERT((ptr) == NULL, message)

// ==================== Тесты для memchr ====================
void test_memchr(void) {
    printf("\n=== Тесты memchr ===\n");
    
    char str[] = "Hello World";
    
    // Тест 1: Найти символ в строке
    void *result = memchr(str, 'o', 11);
    if (result != NULL) {
        printf("PASS: memchr: найти 'o' в 'Hello World'\n");
        tests_passed++;
        int pos = (char*)result - str;
        if (pos == 4) {
            printf("PASS: memchr: позиция 'o' должна быть 4\n");
            tests_passed++;
        } else {
            printf("FAIL: memchr: позиция 'o' должна быть 4\n");
            tests_failed++;
        }
    } else {
        printf("FAIL: memchr: найти 'o' в 'Hello World'\n");
        tests_failed++;
    }
    
    // Тест 2: Символ не найден
    result = memchr(str, 'z', 11);
    ASSERT_NULL(result, "memchr: 'z' не должен быть найден");
    
    // Тест 3: Поиск в части строки
    result = memchr(str, 'W', 11);
    ASSERT_NOT_NULL(result, "memchr: найти 'W' в строке");
    ASSERT_EQ((char*)result - str, 6, "memchr: позиция 'W' должна быть 6");
    
    // Тест 4: Поиск ограничен n байтами
    result = memchr(str, 'W', 5);
    ASSERT_NULL(result, "memchr: 'W' не найден в первых 5 байтах");
    
    // Тест 5: NULL указатель
    result = memchr(NULL, 'a', 5);
    ASSERT_NULL(result, "memchr: NULL указатель должен вернуть NULL");
}

// ==================== Тесты для memcmp ====================
void test_memcmp(void) {
    printf("\n=== Тесты memcmp ===\n");
    
    char str1[] = "Hello";
    char str2[] = "Hello";
    char str3[] = "Hellp";
    char str4[] = "Hella";
    
    // Тест 1: Равные строки
    int result = memcmp(str1, str2, 5);
    ASSERT_EQ(result, 0, "memcmp: равные строки должны вернуть 0");
    
    // Тест 2: Первая строка меньше
    result = memcmp(str1, str3, 5);
    ASSERT(result < 0, "memcmp: 'Hello' < 'Hellp'");
    
    // Тест 3: Первая строка больше
    result = memcmp(str3, str1, 5);
    ASSERT(result > 0, "memcmp: 'Hellp' > 'Hello'");
    
    // Тест 4: Сравнение части строки
    result = memcmp(str1, str4, 4);
    ASSERT_EQ(result, 0, "memcmp: первые 4 символа равны");
    
    // Тест 5: NULL указатели
    result = memcmp(NULL, str2, 5);
    ASSERT_EQ(result, 0, "memcmp: NULL должен вернуть 0");
}

// ==================== Тесты для memcpy ====================
void test_memcpy(void) {
    printf("\n=== Тесты memcpy ===\n");
    
    char src[] = "Hello World";
    char dest[20] = {0};
    
    // Тест 1: Копирование строки
    void *result = memcpy(dest, src, 11);
    ASSERT_PTR_EQ(result, dest, "memcpy: должен вернуть dest");
    int cmp_result = memcmp(dest, src, 11);
    ASSERT_EQ(cmp_result, 0, "memcpy: строки должны совпадать");
    
    // Тест 2: Копирование части
    char dest2[20] = {0};
    memcpy(dest2, src, 5);
    char hello[] = "Hello";
    cmp_result = memcmp(dest2, hello, 5);
    ASSERT_EQ(cmp_result, 0, "memcpy: первые 5 символов");
    
    // Тест 3: NULL указатели
    result = memcpy(NULL, src, 5);
    ASSERT_PTR_EQ(result, NULL, "memcpy: NULL dest должен вернуть NULL");
    
    result = memcpy(dest, NULL, 5);
    ASSERT_PTR_EQ(result, dest, "memcpy: NULL src должен вернуть dest");
}

// ==================== Тесты для memset ====================
void test_memset(void) {
    printf("\n=== Тесты memset ===\n");
    
    char str[20] = "Hello World";
    
    // Тест 1: Заполнение нулями
    void *result = memset(str, 0, 11);
    ASSERT_PTR_EQ(result, str, "memset: должен вернуть str");
    int all_zero = 1;
    for (int i = 0; i < 11; i++) {
        if (str[i] != 0) {
            all_zero = 0;
            break;
        }
    }
    ASSERT(all_zero, "memset: все байты должны быть 0");
    
    // Тест 2: Заполнение символом
    char str2[10];
    memset(str2, 'A', 5);
    int all_a = 1;
    for (int i = 0; i < 5; i++) {
        if (str2[i] != 'A') {
            all_a = 0;
            break;
        }
    }
    ASSERT(all_a, "memset: первые 5 символов должны быть 'A'");
    
    // Тест 3: NULL указатель
    result = memset(NULL, 'A', 5);
    ASSERT_NULL(result, "memset: NULL должен вернуть NULL");
}

// ==================== Тесты для strncat ====================
void test_strncat(void) {
    printf("\n=== Тесты strncat ===\n");
    
    char dest[50] = "Hello";
    char src[] = " World";
    
    // Тест 1: Добавление строки
    char *result = strncat(dest, src, 6);
    ASSERT_PTR_EQ(result, dest, "strncat: должен вернуть dest");
    ASSERT_EQ(my_strcmp(dest, "Hello World"), 0, "strncat: строки должны объединиться");
    
    // Тест 2: Добавление части строки
    char dest2[50] = "Test";
    strncat(dest2, "12345", 3);
    ASSERT_EQ(my_strcmp(dest2, "Test123"), 0, "strncat: первые 3 символа");
    
    // Тест 3: NULL указатели
    result = strncat(NULL, src, 5);
    ASSERT_NULL(result, "strncat: NULL dest должен вернуть NULL");
}

// ==================== Тесты для strchr ====================
void test_strchr(void) {
    printf("\n=== Тесты strchr ===\n");
    
    char str[] = "Hello World";
    
    // Тест 1: Найти первый символ
    char *result = strchr(str, 'H');
    ASSERT_NOT_NULL(result, "strchr: найти 'H'");
    ASSERT_PTR_EQ(result, str, "strchr: 'H' должен быть в начале");
    
    // Тест 2: Найти символ в середине
    result = strchr(str, 'o');
    ASSERT_NOT_NULL(result, "strchr: найти 'o'");
    ASSERT_EQ(result - str, 4, "strchr: первое 'o' в позиции 4");
    
    // Тест 3: Символ не найден
    result = strchr(str, 'z');
    ASSERT_NULL(result, "strchr: 'z' не должен быть найден");
    
    // Тест 4: Поиск нулевого символа
    result = strchr(str, '\0');
    ASSERT_NOT_NULL(result, "strchr: найти '\\0'");
    
    // Тест 5: NULL указатель
    result = strchr(NULL, 'a');
    ASSERT_NULL(result, "strchr: NULL должен вернуть NULL");
}

// ==================== Тесты для strncmp ====================
void test_strncmp(void) {
    printf("\n=== Тесты strncmp ===\n");
    
    char str1[] = "Hello";
    char str2[] = "Hello";
    char str3[] = "Hellp";
    
    // Тест 1: Равные строки
    int result = strncmp(str1, str2, 5);
    ASSERT_EQ(result, 0, "strncmp: равные строки должны вернуть 0");
    
    // Тест 2: Первая меньше
    result = strncmp(str1, str3, 5);
    ASSERT(result < 0, "strncmp: 'Hello' < 'Hellp'");
    
    // Тест 3: Сравнение части
    result = strncmp(str1, str3, 4);
    ASSERT_EQ(result, 0, "strncmp: первые 4 символа равны");
    
    // Тест 4: NULL указатели
    result = strncmp(NULL, str2, 5);
    ASSERT_EQ(result, 0, "strncmp: NULL должен вернуть 0");
}

// ==================== Тесты для strncpy ====================
void test_strncpy(void) {
    printf("\n=== Тесты strncpy ===\n");
    
    char src[] = "Hello";
    char dest[10] = {0};
    
    // Тест 1: Копирование строки
    char *result = strncpy(dest, src, 5);
    ASSERT_PTR_EQ(result, dest, "strncpy: должен вернуть dest");
    int cmp_result = strncmp(dest, src, 5);
    ASSERT_EQ(cmp_result, 0, "strncpy: строки должны совпадать");
    
    // Тест 2: Копирование с дополнением нулями
    char dest2[10];
    memset(dest2, 'X', 10);
    strncpy(dest2, src, 8);
    ASSERT_EQ(dest2[5], '\0', "strncpy: после строки должны быть нули");
    ASSERT_EQ(dest2[6], '\0', "strncpy: после строки должны быть нули");
    
    // Тест 3: NULL указатели
    result = strncpy(NULL, src, 5);
    ASSERT_NULL(result, "strncpy: NULL dest должен вернуть NULL");
}

// ==================== Тесты для strcspn ====================
void test_strcspn(void) {
    printf("\n=== Тесты strcspn ===\n");
    
    char str[] = "Hello World";
    
    // Тест 1: Найти первый символ из набора
    size_t result = strcspn(str, "o");
    ASSERT_EQ(result, 4, "strcspn: длина до 'o' должна быть 4");
    
    // Тест 2: Несколько символов в наборе
    result = strcspn(str, "W ");
    ASSERT_EQ(result, 6, "strcspn: длина до 'W' или пробела должна быть 6");
    
    // Тест 3: Символы не найдены
    size_t str_len = 0;
    while (str[str_len] != '\0') str_len++;
    result = strcspn(str, "xyz");
    ASSERT_EQ(result, str_len, "strcspn: все символы должны быть учтены");
    
    // Тест 4: NULL указатели
    result = strcspn(NULL, "a");
    ASSERT_EQ(result, 0, "strcspn: NULL должен вернуть 0");
}

// ==================== Тесты для strerror ====================
void test_strerror(void) {
    printf("\n=== Тесты strerror ===\n");
    
    // Тест 1: Известные коды ошибок
    char *result = strerror(0);
    ASSERT_NOT_NULL(result, "strerror: код 0 должен вернуть строку");
    
    result = strerror(2);
    ASSERT_NOT_NULL(result, "strerror: код 2 должен вернуть строку");
    
    // Тест 2: Неизвестный код
    result = strerror(999);
    ASSERT_NOT_NULL(result, "strerror: неизвестный код должен вернуть строку");
    int is_unknown = (my_strcmp(result, "Unknown error") == 0);
    ASSERT(is_unknown, "strerror: неизвестный код должен вернуть 'Unknown error'");
}

// ==================== Тесты для strlen ====================
void test_strlen(void) {
    printf("\n=== Тесты strlen ===\n");
    
    // Тест 1: Обычная строка
    char str[] = "Hello";
    size_t result = strlen(str);
    ASSERT_EQ(result, 5, "strlen: 'Hello' должна быть длиной 5");
    
    // Тест 2: Пустая строка
    char empty[] = "";
    result = strlen(empty);
    ASSERT_EQ(result, 0, "strlen: пустая строка должна быть длиной 0");
    
    // Тест 3: Длинная строка
    char long_str[] = "This is a very long string for testing";
    result = strlen(long_str);
    ASSERT_EQ(result, 40, "strlen: длинная строка");
    
    // Тест 4: NULL указатель
    result = strlen(NULL);
    ASSERT_EQ(result, 0, "strlen: NULL должен вернуть 0");
}

// ==================== Тесты для strpbrk ====================
void test_strpbrk(void) {
    printf("\n=== Тесты strpbrk ===\n");
    
    char str[] = "Hello World";
    
    // Тест 1: Найти первый символ из набора
    char *result = strpbrk(str, "oW");
    ASSERT_NOT_NULL(result, "strpbrk: найти 'o' или 'W'");
    ASSERT_EQ(result - str, 4, "strpbrk: первое вхождение 'o' в позиции 4");
    
    // Тест 2: Символы не найдены
    result = strpbrk(str, "xyz");
    ASSERT_NULL(result, "strpbrk: символы не должны быть найдены");
    
    // Тест 3: NULL указатели
    result = strpbrk(NULL, "a");
    ASSERT_NULL(result, "strpbrk: NULL должен вернуть NULL");
}

// ==================== Тесты для strrchr ====================
void test_strrchr(void) {
    printf("\n=== Тесты strrchr ===\n");
    
    char str[] = "Hello World";
    
    // Тест 1: Найти последний символ
    char *result = strrchr(str, 'o');
    ASSERT_NOT_NULL(result, "strrchr: найти последнее 'o'");
    ASSERT_EQ(result - str, 7, "strrchr: последнее 'o' в позиции 7");
    
    // Тест 2: Символ не найден
    result = strrchr(str, 'z');
    ASSERT_NULL(result, "strrchr: 'z' не должен быть найден");
    
    // Тест 3: Поиск последнего символа
    result = strrchr(str, 'd');
    ASSERT_NOT_NULL(result, "strrchr: найти 'd'");
    ASSERT_EQ(result - str, 10, "strrchr: 'd' в позиции 10");
    
    // Тест 4: NULL указатель
    result = strrchr(NULL, 'a');
    ASSERT_NULL(result, "strrchr: NULL должен вернуть NULL");
}

// ==================== Тесты для strstr ====================
void test_strstr(void) {
    printf("\n=== Тесты strstr ===\n");
    
    char haystack[] = "Hello World";
    
    // Тест 1: Найти подстроку
    char *result = strstr(haystack, "World");
    ASSERT_NOT_NULL(result, "strstr: найти 'World'");
    ASSERT_EQ(result - haystack, 6, "strstr: 'World' начинается в позиции 6");
    
    // Тест 2: Подстрока не найдена
    result = strstr(haystack, "xyz");
    ASSERT_NULL(result, "strstr: 'xyz' не должна быть найдена");
    
    // Тест 3: Пустая подстрока
    result = strstr(haystack, "");
    ASSERT_NOT_NULL(result, "strstr: пустая подстрока должна вернуть haystack");
    ASSERT_PTR_EQ(result, haystack, "strstr: пустая подстрока должна вернуть начало");
    
    // Тест 4: NULL указатели
    result = strstr(NULL, "test");
    ASSERT_NULL(result, "strstr: NULL должен вернуть NULL");
}

// ==================== Тесты для strtok ====================
void test_strtok(void) {
    printf("\n=== Тесты strtok ===\n");
    
    // Тест 1: Разбиение строки
    char str1[] = "Hello,World,Test";
    char *token = strtok(str1, ",");
    ASSERT_NOT_NULL(token, "strtok: первый токен должен быть найден");
    if (token != NULL) {
        int cmp = my_strcmp(token, "Hello");
        ASSERT_EQ(cmp, 0, "strtok: первый токен должен быть 'Hello'");
        
        token = strtok(NULL, ",");
        ASSERT_NOT_NULL(token, "strtok: второй токен должен быть найден");
        if (token != NULL) {
            cmp = my_strcmp(token, "World");
            ASSERT_EQ(cmp, 0, "strtok: второй токен должен быть 'World'");
            
            token = strtok(NULL, ",");
            ASSERT_NOT_NULL(token, "strtok: третий токен должен быть найден");
            if (token != NULL) {
                cmp = my_strcmp(token, "Test");
                ASSERT_EQ(cmp, 0, "strtok: третий токен должен быть 'Test'");
                
                token = strtok(NULL, ",");
                ASSERT_NULL(token, "strtok: токенов больше нет");
            }
        }
    }
    
    // Тест 2: Несколько разделителей
    char str2[] = "Hello World";
    token = strtok(str2, " ");
    ASSERT_NOT_NULL(token, "strtok: первый токен должен быть найден");
    if (token != NULL) {
        int cmp = my_strcmp(token, "Hello");
        ASSERT_EQ(cmp, 0, "strtok: первый токен 'Hello'");
    }
    
    // Тест 3: NULL указатель
    token = strtok(NULL, NULL);
    ASSERT_NULL(token, "strtok: NULL delim должен вернуть NULL");
}

// ==================== Главная функция ====================
int main(void) {
    printf("========================================\n");
    printf("  ТЕСТИРОВАНИЕ СТРОКОВЫХ ФУНКЦИЙ\n");
    printf("========================================\n");
    
    // Запускаем все тесты
    printf("Запуск тестов...\n");
    test_memchr();
    printf("memchr OK\n");
    test_memcmp();
    printf("memcmp OK\n");
    test_memcpy();
    printf("memcpy OK\n");
    test_memset();
    printf("memset OK\n");
    test_strncat();
    printf("strncat OK\n");
    test_strchr();
    printf("strchr OK\n");
    test_strncmp();
    printf("strncmp OK\n");
    test_strncpy();
    printf("strncpy OK\n");
    test_strcspn();
    printf("strcspn OK\n");
    test_strerror();
    printf("strerror OK\n");
    test_strlen();
    printf("strlen OK\n");
    test_strpbrk();
    printf("strpbrk OK\n");
    test_strrchr();
    printf("strrchr OK\n");
    test_strstr();
    printf("strstr OK\n");
    test_strtok();
    printf("strtok OK\n");
    
    // Выводим итоги
    printf("\n========================================\n");
    printf("  ИТОГИ ТЕСТИРОВАНИЯ\n");
    printf("========================================\n");
    printf("Пройдено: %d\n", tests_passed);
    printf("Провалено: %d\n", tests_failed);
    printf("Всего: %d\n", tests_passed + tests_failed);
    
    if (tests_failed == 0) {
        printf("\nВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО!\n");
        return 0;
    } else {
        printf("\nЕСТЬ ПРОВАЛЕННЫЕ ТЕСТЫ\n");
        return 1;
    }
}

