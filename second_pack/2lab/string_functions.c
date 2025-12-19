#include "string_functions.h"
#include "error_messages.h"

// Сложность: O(n) - линейный поиск по n байтам
void *memchr(const void *str, int c, size_t n) {
    if (str == NULL) return NULL;
    
    const unsigned char *ptr = (const unsigned char *)str;
    for (size_t i = 0; i < n; i++) {
        if (ptr[i] == (unsigned char)c) {
            return (void *)(ptr + i);
        }
    }
    return NULL;
}

// Сложность: O(n) - сравнение n байтов
int memcmp(const void *str1, const void *str2, size_t n) {
    if (str1 == NULL || str2 == NULL) return 0;
    
    const unsigned char *p1 = (const unsigned char *)str1;
    const unsigned char *p2 = (const unsigned char *)str2;
    
    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }
    return 0;
}

// Сложность: O(n) - копирование n байтов
void *memcpy(void *dest, const void *src, size_t n) {
    if (dest == NULL || src == NULL) return dest;
    
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

// Сложность: O(n) - заполнение n байтов
void *memset(void *str, int c, size_t n) {
    if (str == NULL) return NULL;
    
    unsigned char *ptr = (unsigned char *)str;
    for (size_t i = 0; i < n; i++) {
        ptr[i] = (unsigned char)c;
    }
    return str;
}

// Сложность: O(m + n), где m - длина dest, n - количество добавляемых символов
char *strncat(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL) return dest;
    
    char *d = dest;
    while (*d != '\0') d++;
    
    for (size_t i = 0; i < n && src[i] != '\0'; i++) {
        *d++ = src[i];
    }
    *d = '\0';
    return dest;
}

// Сложность: O(m), где m - длина строки str
char *strchr(const char *str, int c) {
    if (str == NULL) return NULL;
    
    while (*str != '\0') {
        if (*str == (unsigned char)c) {
            return (char *)str;
        }
        str++;
    }
    return ((unsigned char)c == '\0') ? (char *)str : NULL;
}

// Сложность: O(n) - сравнение до n символов
int strncmp(const char *str1, const char *str2, size_t n) {
    if (str1 == NULL || str2 == NULL) return 0;
    
    for (size_t i = 0; i < n; i++) {
        if (str1[i] != str2[i]) {
            return (unsigned char)str1[i] - (unsigned char)str2[i];
        }
        if (str1[i] == '\0') break;
    }
    return 0;
}

// Сложность: O(n) - копирование до n символов
char *strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL) return dest;
    
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

// Сложность: O(m * k), где m - длина str1, k - длина str2
size_t strcspn(const char *str1, const char *str2) {
    if (str1 == NULL || str2 == NULL) return 0;
    
    size_t count = 0;
    while (str1[count] != '\0') {
        const char *reject = str2;
        while (*reject != '\0') {
            if (str1[count] == *reject) {
                return count;
            }
            reject++;
        }
        count++;
    }
    return count;
}

// Сложность: O(1) - доступ к элементу массива по индексу
char *strerror(int errnum) {
    static const char *errors[] = {
        ERROR_MESSAGES
    };
    
    if (errnum >= 0 && errnum <= MAX_ERROR_NUM) {
        return (char *)errors[errnum];
    }
    return "Unknown error";
}

// Сложность: O(m), где m - длина строки str
size_t strlen(const char *str) {
    if (str == NULL) return 0;
    
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

// Сложность: O(m * k), где m - длина str1, k - длина str2
char *strpbrk(const char *str1, const char *str2) {
    if (str1 == NULL || str2 == NULL) return NULL;
    
    while (*str1 != '\0') {
        const char *accept = str2;
        while (*accept != '\0') {
            if (*str1 == *accept) {
                return (char *)str1;
            }
            accept++;
        }
        str1++;
    }
    return NULL;
}

// Сложность: O(m), где m - длина строки str
char *strrchr(const char *str, int c) {
    if (str == NULL) return NULL;
    
    const char *last = NULL;
    while (*str != '\0') {
        if (*str == (unsigned char)c) {
            last = str;
        }
        str++;
    }
    return ((unsigned char)c == '\0') ? (char *)str : (char *)last;
}

// Сложность: O(m * n), где m - длина haystack, n - длина needle (наивный алгоритм)
char *strstr(const char *haystack, const char *needle) {
    if (haystack == NULL || needle == NULL) return NULL;
    if (*needle == '\0') return (char *)haystack;
    
    for (size_t i = 0; haystack[i] != '\0'; i++) {
        size_t j = 0;
        while (needle[j] != '\0' && haystack[i + j] == needle[j]) {
            j++;
        }
        if (needle[j] == '\0') {
            return (char *)(haystack + i);
        }
    }
    return NULL;
}

// Сложность: O(m * k), где m - длина обрабатываемой части строки, k - длина delim
char *strtok(char *str, const char *delim) {
    static char *strtok_save = NULL;
    
    if (delim == NULL) {
        return NULL;
    }
    
    // Если передан новый указатель, начинаем с него
    if (str != NULL) {
        strtok_save = str;
    } 
    // Если str == NULL, продолжаем с сохраненной позиции
    else if (strtok_save == NULL) {
        return NULL;
    }
    
    // Пропускаем все разделители в начале
    char *start = strtok_save;
    while (*start != '\0') {
        const char *d = delim;
        int is_delim = 0;
        while (*d != '\0') {
            if (*start == *d) {
                is_delim = 1;
                break;
            }
            d++;
        }
        if (!is_delim) {
            break; // Нашли начало токена
        }
        start++;
    }
    
    // Если дошли до конца строки, токенов больше нет
    if (*start == '\0') {
        strtok_save = NULL;
        return NULL;
    }
    
    // Ищем конец токена (первый разделитель)
    char *end = start;
    while (*end != '\0') {
        const char *d = delim;
        int is_delim = 0;
        while (*d != '\0') {
            if (*end == *d) {
                is_delim = 1;
                break;
            }
            d++;
        }
        if (is_delim) {
            // Нашли разделитель - заменяем на '\0' и сохраняем позицию
            *end = '\0';
            strtok_save = end + 1;
            return start;
        }
        end++;
    }
    
    // Дошли до конца строки - это последний токен
    strtok_save = NULL;
    return start;
}