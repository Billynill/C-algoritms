#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// Структура вектора (независимая от типа)
typedef struct Vector Vector;

struct Vector {
    void *data;                    // указатель на элементы
    size_t size;                   // текущее количество элементов
    size_t capacity;               // вместимость
    size_t element_size;           // размер одного элемента
    void* (*CopyFunc)(const void*); // функция копирования элемента
    void (*DeleteFunc)(void*);     // функция удаления элемента
};

// Прототипы функций
Vector create_vector(size_t initial_capacity, size_t element_size, 
                    void* (*CopyFunc)(const void*), void (*DeleteFunc)(void*));
void erase_vector(Vector *v);
int is_equal_vector(const Vector *v1, const Vector *v2, 
                   int (*CompareFunc)(const void*, const void*));
void copy_vector(Vector *dest, const Vector *src);
Vector *copy_vector_new(const Vector *src);
void push_back_vector(Vector *v, const void *value);
void delete_at_vector(Vector *v, size_t index);
void* get_at_vector(const Vector *v, size_t index);
void delete_vector(Vector *v);
size_t vector_size(const Vector *v);
size_t vector_capacity(const Vector *v);

#endif
