#include <stdio.h>
#include "string_functions.h"

int main(void) {
    printf("=== Простые тесты строковых функций ===\n\n");
    int passed = 0;
    int failed = 0;
    
    // Тест memchr
    char str[] = "Hello";
    void *result = memchr(str, 'e', 5);
    if (result != NULL && (char*)result - str == 1) {
        printf("memchr: PASS\n");
        passed++;
    } else {
        printf("memchr: FAIL\n");
        failed++;
    }
    
    // Тест memcmp
    char s1[] = "abc";
    char s2[] = "abc";
    if (memcmp(s1, s2, 3) == 0) {
        printf("memcmp: PASS\n");
        passed++;
    } else {
        printf("memcmp: FAIL\n");
        failed++;
    }
    
    // Тест memcpy
    char src[] = "test";
    char dest[10] = {0};
    memcpy(dest, src, 4);
    int ok = 1;
    for (int i = 0; i < 4; i++) {
        if (dest[i] != src[i]) {
            ok = 0;
            break;
        }
    }
    if (ok) {
        printf("memcpy: PASS\n");
        passed++;
    } else {
        printf("memcpy: FAIL\n");
        failed++;
    }
    
    // Тест memset
    char buf[10];
    memset(buf, 'A', 5);
    ok = 1;
    for (int i = 0; i < 5; i++) {
        if (buf[i] != 'A') {
            ok = 0;
            break;
        }
    }
    if (ok) {
        printf("memset: PASS\n");
        passed++;
    } else {
        printf("memset: FAIL\n");
        failed++;
    }
    
    // Тест strlen
    if (strlen("Hello") == 5) {
        printf("strlen: PASS\n");
        passed++;
    } else {
        printf("strlen: FAIL\n");
        failed++;
    }
    
    // Тест strchr
    char *p = strchr("Hello", 'e');
    if (p != NULL && *p == 'e') {
        printf("strchr: PASS\n");
        passed++;
    } else {
        printf("strchr: FAIL\n");
        failed++;
    }
    
    printf("\nИтого: %d пройдено, %d провалено\n", passed, failed);
    return failed == 0 ? 0 : 1;
}



