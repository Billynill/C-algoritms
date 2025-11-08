#include "utils.h"
#include <string.h>
#include <stdlib.h>

bool is_latin_letter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool is_arabic_digit(char c) {
    return c >= '0' && c <= '9';
}

bool is_special_char(char c) {
    return !is_latin_letter(c) && !is_arabic_digit(c) && c != ' ' && c != '\n' && c != '\r' && c != '\t';
}

char* generate_output_filename(const char* input_filename) {
    if (input_filename == NULL) return NULL;
    
    const char* prefix = "out_";
    size_t len = strlen(prefix) + strlen(input_filename) + 1;
    char* result = malloc(len);
    
    if (result != NULL) {
        strcpy(result, prefix);
        strcat(result, input_filename);
    }
    
    return result;
}