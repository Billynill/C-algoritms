#include "interpreter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int power(int base, int exponent) {
    if (exponent < 0) return 0;
    if (exponent == 0) return 1;
    
    int result = 1;
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result *= base;
        }
        base *= base;
        exponent /= 2;
    }
    return result;
}

InterpreterStatus initialize_interpreter(InterpreterState* state, const char* log_filename) {
    if (state == NULL || log_filename == NULL) {
        return ERROR_INVALID_SYNTAX;
    }
    
    for (int i = 0; i < VARIABLES_COUNT; i++) {
        state->variables[i] = 0;
        state->initialized[i] = 0;
    }
    
    state->line_number = 0;
    
    state->log_file = fopen(log_filename, "w");
    if (state->log_file == NULL) {
        return ERROR_FILE_OPERATION;
    }
    
    fprintf(state->log_file, "=== НАЧАЛО ТРАССИРОВКИ ===\n");
    return SUCCESS;
}

void cleanup_interpreter(InterpreterState* state) {
    if (state == NULL) return;
    
    if (state->log_file != NULL) {
        fprintf(state->log_file, "=== КОНЕЦ ТРАССИРОВКИ ===\n");
        fclose(state->log_file);
        state->log_file = NULL;
    }
}

int is_valid_variable_name(char name) {
    return name >= 'A' && name <= 'Z';
}

int get_variable_index(char name) {
    if (is_valid_variable_name(name)) {
        return name - 'A';
    }
    return -1;
}

static InterpreterStatus parse_operand(const char* str, const InterpreterState* state, int* result) {
    if (str == NULL || result == NULL) {
        return ERROR_INVALID_SYNTAX;
    }
    
    if (strlen(str) == 1 && is_valid_variable_name(str[0])) {
        int var_index = get_variable_index(str[0]);
        if (var_index == -1 || !state->initialized[var_index]) {
            return ERROR_UNKNOWN_VARIABLE;
        }
        *result = state->variables[var_index];
        return SUCCESS;
    }
    
    char* endptr;
    long value = strtol(str, &endptr, 10);
    if (*endptr == '\0') {
        *result = (int)value;
        return SUCCESS;
    }
    
    return ERROR_INVALID_SYNTAX;
}

static InterpreterStatus parse_expression(const char** expr_ptr, const InterpreterState* state, int* result) {
    if (expr_ptr == NULL || *expr_ptr == NULL || result == NULL) {
        return ERROR_INVALID_SYNTAX;
    }
    
    const char* expr = *expr_ptr;
    while (isspace((unsigned char)*expr)) expr++;
    
    char operand[32] = {0};
    int i = 0;
    
    int is_negative = 0;
    if (*expr == '-') {
        is_negative = 1;
        expr++;
        while (isspace((unsigned char)*expr)) expr++;
    }
    
    while (*expr != '\0' && !isspace((unsigned char)*expr) && 
           *expr != '+' && *expr != '-' && *expr != '*' && 
           *expr != '/' && *expr != '^' && *expr != ')' && i < 31) {
        operand[i++] = *expr++;
    }
    operand[i] = '\0';
    
    if (operand[0] == '\0') {
        return ERROR_INVALID_SYNTAX;
    }
    
    int value;
    InterpreterStatus status = parse_operand(operand, state, &value);
    if (status != SUCCESS) return status;
    
    if (is_negative) {
        value = -value;
    }
    
    while (isspace((unsigned char)*expr)) expr++;
    
    if (*expr == '\0' || *expr == ')') {
        *expr_ptr = expr;
        *result = value;
        return SUCCESS;
    }
    
    while (*expr != '\0' && *expr != ')') {
        char op = *expr++;
        
        while (isspace((unsigned char)*expr)) expr++;
        
        int right_value;
        status = parse_expression(&expr, state, &right_value);
        if (status != SUCCESS) return status;
        
        switch (op) {
            case '+': value = value + right_value; break;
            case '-': value = value - right_value; break;
            case '*': value = value * right_value; break;
            case '/': 
                if (right_value == 0) return ERROR_DIVISION_BY_ZERO;
                value = value / right_value; 
                break;
            case '^': value = power(value, right_value); break;
            default: return ERROR_INVALID_SYNTAX;
        }
        
        while (isspace((unsigned char)*expr)) expr++;
    }
    
    *expr_ptr = expr;
    *result = value;
    return SUCCESS;
}

static InterpreterStatus evaluate_expression(const char* expr, const InterpreterState* state, int* result) {
    if (expr == NULL || result == NULL) {
        return ERROR_INVALID_SYNTAX;
    }
    
    const char* expr_ptr = expr;
    return parse_expression(&expr_ptr, state, result);
}

static void log_state(InterpreterState* state, const char* original_line, const char* operation_desc) {
    if (state == NULL || state->log_file == NULL) return;
    
    fprintf(state->log_file, "[%zu] %s | ", state->line_number, original_line);
    
    int first = 1;
    for (int i = 0; i < VARIABLES_COUNT; i++) {
        if (state->initialized[i]) {
            if (!first) fprintf(state->log_file, ", ");
            fprintf(state->log_file, "%c=%d", 'A' + i, state->variables[i]);
            first = 0;
        }
    }
    if (first) fprintf(state->log_file, "No variables");
    
    fprintf(state->log_file, " | %s\n", operation_desc);
    fflush(state->log_file);
}

static InterpreterStatus process_assignment(InterpreterState* state, const char* line) {
    char var_name;
    char expression[MAX_EXPRESSION_LENGTH];
    
    if (sscanf(line, " %c = %255[^\n]", &var_name, expression) != 2) {
        return ERROR_INVALID_SYNTAX;
    }
    
    if (!is_valid_variable_name(var_name)) {
        return ERROR_INVALID_SYNTAX;
    }
    
    int result;
    InterpreterStatus status = evaluate_expression(expression, state, &result);
    if (status != SUCCESS) return status;
    
    int var_index = get_variable_index(var_name);
    state->variables[var_index] = result;
    state->initialized[var_index] = 1;
    
    log_state(state, line, "Assignment");
    return SUCCESS;
}

static InterpreterStatus process_print(InterpreterState* state, const char* line) {
    char var_name;
    
    if (sscanf(line, "print ( %c )", &var_name) != 1) {
        if (sscanf(line, "print(%c)", &var_name) != 1) {
            return ERROR_INVALID_SYNTAX;
        }
    }
    
    if (!is_valid_variable_name(var_name)) {
        return ERROR_INVALID_SYNTAX;
    }
    
    int var_index = get_variable_index(var_name);
    if (var_index == -1 || !state->initialized[var_index]) {
        return ERROR_UNKNOWN_VARIABLE;
    }
    
    printf("%d\n", state->variables[var_index]);
    log_state(state, line, "Print");
    return SUCCESS;
}

InterpreterStatus process_line(InterpreterState* state, const char* line) {
    if (state == NULL || line == NULL) {
        return ERROR_INVALID_SYNTAX;
    }
    
    state->line_number++;
    
    int is_empty = 1;
    for (size_t i = 0; line[i] != '\0'; i++) {
        if (!isspace((unsigned char)line[i])) {
            if (line[i] == '#') break;
            is_empty = 0;
            break;
        }
    }
    if (is_empty) return SUCCESS;
    
    char first_word[32] = {0};
    if (sscanf(line, " %31s", first_word) != 1) {
        return ERROR_INVALID_SYNTAX;
    }
    
    if (strlen(first_word) == 1 && is_valid_variable_name(first_word[0]) && 
        strchr(line, '=') != NULL) {
        return process_assignment(state, line);
    }
    
    if (strncmp(first_word, "print", 5) == 0) {
        return process_print(state, line);
    }
    
    return ERROR_INVALID_SYNTAX;
}

InterpreterStatus process_file(InterpreterState* state, const char* filename) {
    if (state == NULL || filename == NULL) {
        return ERROR_INVALID_SYNTAX;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return ERROR_FILE_OPERATION;
    }
    
    char line[MAX_LINE_LENGTH];
    InterpreterStatus overall_status = SUCCESS;
    
    while (fgets(line, sizeof(line), file) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        InterpreterStatus status = process_line(state, line);
        if (status != SUCCESS && overall_status == SUCCESS) {
            overall_status = status;
            fprintf(stderr, "Ошибка в строке %zu: %s\n", state->line_number, line);
        }
    }
    
    fclose(file);
    return overall_status;
}

void print_variables(const InterpreterState* state) {
    if (state == NULL) return;
    
    printf("Текущие переменные:\n");
    for (int i = 0; i < VARIABLES_COUNT; i++) {
        if (state->initialized[i]) {
            printf("  %c = %d\n", 'A' + i, state->variables[i]);
        }
    }
}
