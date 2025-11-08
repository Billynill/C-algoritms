#ifndef OVERPRINTF_H
#define OVERPRINTF_H

#include <stdio.h>
#include <stdarg.h>

int overfprintf(FILE *stream, const char *format, ...);
int oversprintf(char *str, const char *format, ...);

int handle_roman(char *buffer, int value);
int handle_zeckendorf(char *buffer, unsigned int value);
int handle_custom_base(char *buffer, int value, int base, int uppercase);
int handle_to_decimal(char *buffer, const char *str, int base, int uppercase);
int handle_memory_dump_int(char *buffer, int value);
int handle_memory_dump_uint(char *buffer, unsigned int value);
int handle_memory_dump_double(char *buffer, double value);
int handle_memory_dump_float(char *buffer, float value);

#endif