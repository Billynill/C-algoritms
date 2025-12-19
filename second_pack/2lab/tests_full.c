#include <stdio.h>
#include "string_functions.h"

static int tests_passed = 0;
static int tests_failed = 0;

static void test_pass(const char *msg) {
    printf("PASS: %s\n", msg);
    tests_passed++;
}

static void test_fail(const char *msg) {
    printf("FAIL: %s\n", msg);
    tests_failed++;
}

// Вспомогательная функция для сравнения строк
static int my_strcmp(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) return -1;
    while (*s1 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

void test_memchr(void) {
    printf("\n=== Тесты memchr ===\n");
    char str[] = "Hello World";
    
    void *result = memchr(str, 'o', 11);
    if (result != NULL && (char*)result - str == 4) {
        test_pass("memchr: найти 'o' в позиции 4");
    } else {
        test_fail("memchr: найти 'o'");
    }
    
    result = memchr(str, 'z', 11);
    if (result == NULL) {
        test_pass("memchr: 'z' не найден");
    } else {
        test_fail("memchr: 'z' не должен быть найден");
    }
    
    result = memchr(NULL, 'a', 5);
    if (result == NULL) {
        test_pass("memchr: NULL указатель");
    } else {
        test_fail("memchr: NULL должен вернуть NULL");
    }
}

void test_memcmp(void) {
    printf("\n=== Тесты memcmp ===\n");
    char s1[] = "Hello";
    char s2[] = "Hello";
    char s3[] = "Hellp";
    
    if (memcmp(s1, s2, 5) == 0) {
        test_pass("memcmp: равные строки");
    } else {
        test_fail("memcmp: равные строки");
    }
    
    if (memcmp(s1, s3, 5) < 0) {
        test_pass("memcmp: первая строка меньше");
    } else {
        test_fail("memcmp: сравнение строк");
    }
}

void test_memcpy(void) {
    printf("\n=== Тесты memcpy ===\n");
    char src[] = "Hello";
    char dest[10] = {0};
    
    void *result = memcpy(dest, src, 5);
    if (result == dest) {
        int ok = 1;
        for (int i = 0; i < 5; i++) {
            if (dest[i] != src[i]) {
                ok = 0;
                break;
            }
        }
        if (ok) {
            test_pass("memcpy: копирование строки");
        } else {
            test_fail("memcpy: копирование строки");
        }
    } else {
        test_fail("memcpy: возврат указателя");
    }
}

void test_memset(void) {
    printf("\n=== Тесты memset ===\n");
    char buf[10];
    
    void *result = memset(buf, 'A', 5);
    if (result == buf) {
        int ok = 1;
        for (int i = 0; i < 5; i++) {
            if (buf[i] != 'A') {
                ok = 0;
                break;
            }
        }
        if (ok) {
            test_pass("memset: заполнение символом");
        } else {
            test_fail("memset: заполнение символом");
        }
    } else {
        test_fail("memset: возврат указателя");
    }
}

void test_strncat(void) {
    printf("\n=== Тесты strncat ===\n");
    char dest[50] = "Hello";
    char src[] = " World";
    
    char *result = strncat(dest, src, 6);
    if (result == dest && my_strcmp(dest, "Hello World") == 0) {
        test_pass("strncat: объединение строк");
    } else {
        test_fail("strncat: объединение строк");
    }
}

void test_strchr(void) {
    printf("\n=== Тесты strchr ===\n");
    char str[] = "Hello";
    
    char *result = strchr(str, 'e');
    if (result != NULL && result - str == 1) {
        test_pass("strchr: найти 'e'");
    } else {
        test_fail("strchr: найти 'e'");
    }
    
    result = strchr(str, 'z');
    if (result == NULL) {
        test_pass("strchr: символ не найден");
    } else {
        test_fail("strchr: символ не должен быть найден");
    }
}

void test_strncmp(void) {
    printf("\n=== Тесты strncmp ===\n");
    char s1[] = "Hello";
    char s2[] = "Hello";
    
    if (strncmp(s1, s2, 5) == 0) {
        test_pass("strncmp: равные строки");
    } else {
        test_fail("strncmp: равные строки");
    }
}

void test_strncpy(void) {
    printf("\n=== Тесты strncpy ===\n");
    char src[] = "Hello";
    char dest[10] = {0};
    
    char *result = strncpy(dest, src, 5);
    if (result == dest) {
        int ok = 1;
        for (int i = 0; i < 5; i++) {
            if (dest[i] != src[i]) {
                ok = 0;
                break;
            }
        }
        if (ok) {
            test_pass("strncpy: копирование");
        } else {
            test_fail("strncpy: копирование");
        }
    } else {
        test_fail("strncpy: возврат указателя");
    }
}

void test_strcspn(void) {
    printf("\n=== Тесты strcspn ===\n");
    char str[] = "Hello";
    
    size_t result = strcspn(str, "o");
    if (result == 4) {
        test_pass("strcspn: длина до 'o'");
    } else {
        test_fail("strcspn: длина до 'o'");
    }
}

void test_strerror(void) {
    printf("\n=== Тесты strerror ===\n");
    char *result = strerror(0);
    if (result != NULL) {
        test_pass("strerror: код 0");
    } else {
        test_fail("strerror: код 0");
    }
    
    result = strerror(999);
    if (result != NULL && my_strcmp(result, "Unknown error") == 0) {
        test_pass("strerror: неизвестный код");
    } else {
        test_fail("strerror: неизвестный код");
    }
}

void test_strlen(void) {
    printf("\n=== Тесты strlen ===\n");
    if (strlen("Hello") == 5) {
        test_pass("strlen: длина строки");
    } else {
        test_fail("strlen: длина строки");
    }
    
    if (strlen("") == 0) {
        test_pass("strlen: пустая строка");
    } else {
        test_fail("strlen: пустая строка");
    }
    
    if (strlen(NULL) == 0) {
        test_pass("strlen: NULL указатель");
    } else {
        test_fail("strlen: NULL указатель");
    }
}

void test_strpbrk(void) {
    printf("\n=== Тесты strpbrk ===\n");
    char str[] = "Hello";
    
    char *result = strpbrk(str, "eo");
    if (result != NULL && result - str == 1) {
        test_pass("strpbrk: найти символ из набора");
    } else {
        test_fail("strpbrk: найти символ из набора");
    }
}

void test_strrchr(void) {
    printf("\n=== Тесты strrchr ===\n");
    char str[] = "Hello";
    
    char *result = strrchr(str, 'l');
    if (result != NULL && result - str == 3) {
        test_pass("strrchr: последнее вхождение 'l'");
    } else {
        test_fail("strrchr: последнее вхождение 'l'");
    }
}

void test_strstr(void) {
    printf("\n=== Тесты strstr ===\n");
    char haystack[] = "Hello World";
    
    char *result = strstr(haystack, "World");
    if (result != NULL && result - haystack == 6) {
        test_pass("strstr: найти подстроку");
    } else {
        test_fail("strstr: найти подстроку");
    }
    
    result = strstr(haystack, "xyz");
    if (result == NULL) {
        test_pass("strstr: подстрока не найдена");
    } else {
        test_fail("strstr: подстрока не должна быть найдена");
    }
}

void test_strtok(void) {
    printf("\n=== Тесты strtok ===\n");
    
    // Тест 1: Разбиение строки на токены
    char str1[] = "Hello,World,Test";
    char *token = strtok(str1, ",");
    if (token != NULL && my_strcmp(token, "Hello") == 0) {
        test_pass("strtok: первый токен");
    } else {
        test_fail("strtok: первый токен");
    }
    
    token = strtok(NULL, ",");
    if (token != NULL && my_strcmp(token, "World") == 0) {
        test_pass("strtok: второй токен");
    } else {
        test_fail("strtok: второй токен");
    }
    
    token = strtok(NULL, ",");
    if (token != NULL && my_strcmp(token, "Test") == 0) {
        test_pass("strtok: третий токен");
    } else {
        test_fail("strtok: третий токен");
    }
    
    token = strtok(NULL, ",");
    if (token == NULL) {
        test_pass("strtok: токенов больше нет");
    } else {
        test_fail("strtok: токенов больше нет");
    }
    
    // Тест 2: Несколько разделителей
    char str2[] = "Hello World";
    token = strtok(str2, " ");
    if (token != NULL && my_strcmp(token, "Hello") == 0) {
        test_pass("strtok: несколько разделителей");
    } else {
        test_fail("strtok: несколько разделителей");
    }
    
    // Тест 3: NULL delim
    token = strtok(NULL, NULL);
    if (token == NULL) {
        test_pass("strtok: NULL delim");
    } else {
        test_fail("strtok: NULL delim");
    }
}

int main(void) {
    printf("========================================\n");
    printf("  ТЕСТИРОВАНИЕ СТРОКОВЫХ ФУНКЦИЙ\n");
    printf("========================================\n");
    
    test_memchr();
    test_memcmp();
    test_memcpy();
    test_memset();
    test_strncat();
    test_strchr();
    test_strncmp();
    test_strncpy();
    test_strcspn();
    test_strerror();
    test_strlen();
    test_strpbrk();
    test_strrchr();
    test_strstr();
    test_strtok();
    
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

