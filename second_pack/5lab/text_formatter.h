#ifndef TEXT_FORMATTER_H
#define TEXT_FORMATTER_H

#include <stdio.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 80

// Основные функции
bool format_file(const char *input_path, const char *output_path);
int split_line(char *line, FILE *output);
bool is_word_character(char c);
int find_word_end(const char *text, int start);
int find_word_start(const char *text, int start);

#endif