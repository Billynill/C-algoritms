#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stddef.h>
#include <stdio.h>

#define VARIABLES_COUNT 26  // A-Z
#define MAX_EXPRESSION_LENGTH 256
#define MAX_LINE_LENGTH 512

typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_SYNTAX = 1,
    ERROR_UNKNOWN_VARIABLE = 2,
    ERROR_DIVISION_BY_ZERO = 3,
    ERROR_INVALID_EXPRESSION = 4,
    ERROR_FILE_OPERATION = 5
} InterpreterStatus;

typedef struct {
    int variables[VARIABLES_COUNT];
    int initialized[VARIABLES_COUNT];
    size_t line_number;
    FILE* log_file;
} InterpreterState;

InterpreterStatus initialize_interpreter(InterpreterState* state, const char* log_filename);
void cleanup_interpreter(InterpreterState* state);
InterpreterStatus process_line(InterpreterState* state, const char* line);
InterpreterStatus process_file(InterpreterState* state, const char* filename);
void print_variables(const InterpreterState* state);
int get_variable_index(char name);
int is_valid_variable_name(char name);

#endif
