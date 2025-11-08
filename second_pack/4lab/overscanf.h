#ifndef OVERSCANF_H
#define OVERSCANF_H

#include <stdio.h>
#include <stdarg.h>

int overfscanf(FILE *stream, const char *format, ...);
int oversscanf(const char *str, const char *format, ...);

int parse_roman(const char *str, int *value);
int parse_zeckendorf(const char *str, unsigned int *value);
int parse_custom_base(const char *str, int *value, int base, int uppercase);

#endif