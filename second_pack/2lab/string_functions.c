#include "string_functions.h"

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

void *memcpy(void *dest, const void *src, size_t n) {
    if (dest == NULL || src == NULL) return dest;
    
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

void *memset(void *str, int c, size_t n) {
    if (str == NULL) return NULL;
    
    unsigned char *ptr = (unsigned char *)str;
    for (size_t i = 0; i < n; i++) {
        ptr[i] = (unsigned char)c;
    }
    return str;
}

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

char *strchr(const char *str, int c) {
    if (str == NULL) return NULL;
    
    while (*str != '\0') {
        if (*str == (char)c) {
            return (char *)str;
        }
        str++;
    }
    return ((char)c == '\0') ? (char *)str : NULL;
}

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

char *strerror(int errnum) {
    static const char *errors[] = {
        "No error",                                     // 0
        "Operation not permitted",                      // 1
        "No such file or directory",                    // 2
        "No such process",                              // 3
        "Interrupted system call",                      // 4
        "Input/output error",                           // 5
        "Device not configured",                        // 6
        "Argument list too long",                       // 7
        "Exec format error",                            // 8
        "Bad file descriptor",                          // 9
        "No child processes",                           // 10
        "Resource deadlock avoided",                    // 11
        "Cannot allocate memory",                       // 12
        "Permission denied",                            // 13
        "Bad address",                                  // 14
        "Block device required",                        // 15
        "Resource busy",                                // 16
        "File exists",                                  // 17
        "Cross-device link",                            // 18
        "Operation not supported by device",            // 19
        "Not a directory",                              // 20
        "Is a directory",                               // 21
        "Invalid argument",                             // 22
        "Too many open files in system",                // 23
        "Too many open files",                          // 24
        "Inappropriate ioctl for device",               // 25
        "Text file busy",                               // 26
        "File too large",                               // 27
        "No space left on device",                      // 28
        "Illegal seek",                                 // 29
        "Read-only file system",                        // 30
        "Too many links",                               // 31
        "Broken pipe",                                  // 32
        "Numerical argument out of domain",             // 33
        "Result too large",                             // 34
        "Resource temporarily unavailable",             // 35
        "Operation now in progress",                    // 36
        "Operation already in progress",                // 37
        "Socket operation on non-socket",               // 38
        "Destination address required",                 // 39
        "Message too long",                             // 40
        "Protocol wrong type for socket",               // 41
        "Protocol not available",                       // 42
        "Protocol not supported",                       // 43
        "Socket type not supported",                    // 44
        "Operation not supported",                      // 45
        "Protocol family not supported",                // 46
        "Address family not supported by protocol family", // 47
        "Address already in use",                       // 48
        "Can't assign requested address",               // 49
        "Network is down",                              // 50
        "Network is unreachable",                       // 51
        "Network dropped connection on reset",          // 52
        "Software caused connection abort",             // 53
        "Connection reset by peer",                     // 54
        "No buffer space available",                    // 55
        "Socket is already connected",                  // 56
        "Socket is not connected",                      // 57
        "Can't send after socket shutdown",             // 58
        "Too many references: can't splice",            // 59
        "Operation timed out",                          // 60
        "Connection refused",                           // 61
        "Too many levels of symbolic links",            // 62
        "File name too long",                           // 63
        "Host is down",                                 // 64
        "No route to host",                             // 65
        "Directory not empty",                          // 66
        "Too many processes",                           // 67
        "Too many users",                               // 68
        "Disc quota exceeded",                          // 69
        "Stale NFS file handle",                        // 70
        "Too many levels of remote in path",            // 71
        "RPC struct is bad",                            // 72
        "RPC version wrong",                            // 73
        "RPC prog. not avail",                          // 74
        "Program version wrong",                        // 75
        "Bad procedure for program",                    // 76
        "No locks available",                           // 77
        "Function not implemented",                     // 78
        "Inappropriate file type or format",            // 79
        "Authentication error",                         // 80
        "Need authenticator",                           // 81
        "Device power is off",                          // 82
        "Device error",                                 // 83
        "Value too large to be stored in data type",    // 84
        "Bad executable (or shared library)",           // 85
        "Bad CPU type in executable",                   // 86
        "Shared library version mismatch",              // 87
        "Malformed Mach-o file",                        // 88
        "Operation canceled",                           // 89
        "Identifier removed",                           // 90
        "No message of desired type",                   // 91
        "Illegal byte sequence",                        // 92
        "Attribute not found",                          // 93
        "Bad message",                                  // 94
        "EMULTIHOP (Reserved)",                         // 95
        "No message available",                         // 96
        "ENOLINK (Reserved)",                           // 97
        "No STREAM resources",                          // 98
        "Not a STREAM",                                 // 99
        "Protocol error",                               // 100
        "STREAM ioctl timeout",                         // 101
        "Operation not supported on socket",            // 102
        "Policy not found",                             // 103
        "State not recoverable",                        // 104
        "Previous owner died",                          // 105
        "Interface output queue is full"                // 106
    };
    
    if (errnum >= 0 && errnum <= 106) {
        return (char *)errors[errnum];
    }
    return "Unknown error";
}

size_t strlen(const char *str) {
    if (str == NULL) return 0;
    
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

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

char *strrchr(const char *str, int c) {
    if (str == NULL) return NULL;
    
    const char *last = NULL;
    while (*str != '\0') {
        if (*str == (char)c) {
            last = str;
        }
        str++;
    }
    return ((char)c == '\0') ? (char *)str : (char *)last;
}

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

static char *strtok_save = NULL;

char *strtok(char *str, const char *delim) {
    if (delim == NULL) return NULL;
    
    if (str != NULL) {
        strtok_save = str;
    } else if (strtok_save == NULL) {
        return NULL;
    }
    
    char *start = strtok_save;
    while (*start != '\0') {
        const char *d = delim;
        while (*d != '\0') {
            if (*start == *d) {
                start++;
                break;
            }
            d++;
        }
        if (*d == '\0') break;
    }
    
    if (*start == '\0') {
        strtok_save = NULL;
        return NULL;
    }
    
    char *end = start;
    while (*end != '\0') {
        const char *d = delim;
        while (*d != '\0') {
            if (*end == *d) {
                *end = '\0';
                strtok_save = end + 1;
                return start;
            }
            d++;
        }
        end++;
    }
    
    strtok_save = NULL;
    return start;
}