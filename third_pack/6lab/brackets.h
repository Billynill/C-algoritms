#ifndef BRACKETS_H
#define BRACKETS_H

#include <stddef.h>

typedef enum {
    BRACKETS_OK = 0,
    BRACKETS_UNBALANCED = 1,
    BRACKETS_EMPTY_STRING = 2,
    BRACKETS_INVALID_POINTER = 3
} BracketStatus;

BracketStatus check_brackets(const char *str);

#endif
