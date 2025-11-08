#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

bool is_latin_letter(char c);
bool is_arabic_digit(char c);
bool is_special_char(char c);
char* generate_output_filename(const char* input_filename);

#endif