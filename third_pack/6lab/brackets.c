#include "brackets.h"
#include <stdlib.h>

typedef struct {
    char *data;
    size_t size;
    size_t capacity;
} CharStack;

static CharStack create_stack(size_t initial_capacity) {
    CharStack stack = {NULL, 0, 0};
    if (initial_capacity > 0) {
        stack.data = malloc(initial_capacity * sizeof(char));
        if (stack.data != NULL) {
            stack.capacity = initial_capacity;
        }
    }
    return stack;
}

static void delete_stack(CharStack *stack) {
    if (stack != NULL && stack->data != NULL) {
        free(stack->data);
        stack->data = NULL;
        stack->size = 0;
        stack->capacity = 0;
    }
}

static int stack_push(CharStack *stack, char value) {
    if (stack == NULL) return 0;
    
    if (stack->size >= stack->capacity) {
        size_t new_capacity = stack->capacity == 0 ? 1 : stack->capacity * 2;
        char *new_data = realloc(stack->data, new_capacity * sizeof(char));
        if (new_data == NULL) return 0;
        
        stack->data = new_data;
        stack->capacity = new_capacity;
    }
    
    stack->data[stack->size++] = value;
    return 1;
}

static char stack_pop(CharStack *stack) {
    if (stack == NULL || stack->size == 0) return '\0';
    return stack->data[--stack->size];
}

static int is_open_bracket(char c) {
    return c == '(' || c == '[' || c == '{' || c == '<';
}

static int is_close_bracket(char c) {
    return c == ')' || c == ']' || c == '}' || c == '>';
}

static int brackets_match(char open, char close) {
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}') ||
           (open == '<' && close == '>');
}

BracketStatus check_brackets(const char *str) {
    if (str == NULL) return BRACKETS_INVALID_POINTER;
    
    CharStack stack = create_stack(10);
    if (stack.data == NULL) return BRACKETS_UNBALANCED;
    
    BracketStatus result = BRACKETS_OK;
    
    for (size_t i = 0; str[i] != '\0'; i++) {
        char current = str[i];
        
        if (is_open_bracket(current)) {
            if (!stack_push(&stack, current)) {
                result = BRACKETS_UNBALANCED;
                break;
            }
        } else if (is_close_bracket(current)) {
            if (stack.size == 0) {
                result = BRACKETS_UNBALANCED;
                break;
            }
            
            char last_open = stack_pop(&stack);
            if (!brackets_match(last_open, current)) {
                result = BRACKETS_UNBALANCED;
                break;
            }
        }
    }
    
    if (result == BRACKETS_OK && stack.size > 0) {
        result = BRACKETS_UNBALANCED;
    }
    
    if (result == BRACKETS_OK && str[0] == '\0') {
        result = BRACKETS_EMPTY_STRING;
    }
    
    delete_stack(&stack);
    return result;
}
