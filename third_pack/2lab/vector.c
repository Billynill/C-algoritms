#include "vector.h"
#include <stdio.h>

// Создание нового вектора
Vector create_vector(size_t initial_capacity, size_t element_size, 
                    void* (*CopyFunc)(const void*), void (*DeleteFunc)(void*)) {
    Vector v = {NULL, 0, 0, element_size, CopyFunc, DeleteFunc};
    
    if (initial_capacity > 0 && element_size > 0) {
        v.data = malloc(initial_capacity * element_size);
        if (v.data != NULL) {
            v.capacity = initial_capacity;
        }
    }
    
    return v;
}

// Удаление внутреннего содержимого вектора
void erase_vector(Vector *v) {
    if (v == NULL) return;
    
    if (v->data != NULL && v->DeleteFunc != NULL) {
        for (size_t i = 0; i < v->size; i++) {
            void *element = (char*)v->data + i * v->element_size;
            v->DeleteFunc(element);
        }
        free(v->data);
        v->data = NULL;
    }
    
    v->size = 0;
    v->capacity = 0;
}

// Вспомогательная функция для изменения capacity
static int vector_reserve(Vector *v, size_t new_capacity) {
    if (v == NULL || v->element_size == 0) return 0;
    
    if (new_capacity <= v->capacity) return 1;
    
    void *new_data = realloc(v->data, new_capacity * v->element_size);
    if (new_data == NULL) return 0;
    
    v->data = new_data;
    v->capacity = new_capacity;
    return 1;
}

// Сравнение двух векторов (лексикографически)
int is_equal_vector(const Vector *v1, const Vector *v2, 
                   int (*CompareFunc)(const void*, const void*)) {
    if (v1 == NULL || v2 == NULL || CompareFunc == NULL) return 0;
    if (v1->size != v2->size || v1->element_size != v2->element_size) return 0;
    
    for (size_t i = 0; i < v1->size; i++) {
        void *elem1 = (char*)v1->data + i * v1->element_size;
        void *elem2 = (char*)v2->data + i * v2->element_size;
        
        if (CompareFunc(elem1, elem2) != 0) {
            return 0;
        }
    }
    
    return 1;
}

// Копирование содержимого одного вектора в другой (существующий)
void copy_vector(Vector *dest, const Vector *src) {
    if (dest == NULL || src == NULL || dest->CopyFunc == NULL) return;
    if (dest->element_size != src->element_size) return;
    
    // Очищаем целевой вектор
    erase_vector(dest);
    
    // Резервируем память
    if (!vector_reserve(dest, src->size)) return;
    
    // Копируем элементы
    for (size_t i = 0; i < src->size; i++) {
        void *src_element = (char*)src->data + i * src->element_size;
        void *dest_element = (char*)dest->data + i * dest->element_size;
        
        // Копируем данные напрямую, предполагая что CopyFunc делает глубокое копирование если нужно
        void *copied = dest->CopyFunc(src_element);
        if (copied != NULL) {
            memcpy(dest_element, copied, dest->element_size);
            // Если CopyFunc выделял память, освобождаем временный объект
            if (copied != src_element) { // проверяем, что это не тот же указатель
                dest->DeleteFunc(copied);
            }
        }
    }
    dest->size = src->size;
}

// Создание нового вектора в динамической памяти и копирование содержимого
Vector *copy_vector_new(const Vector *src) {
    if (src == NULL) return NULL;
    
    Vector *new_vec = malloc(sizeof(Vector));
    if (new_vec == NULL) return NULL;
    
    *new_vec = create_vector(src->size, src->element_size, src->CopyFunc, src->DeleteFunc);
    copy_vector(new_vec, src);
    
    return new_vec;
}

// Добавление элемента в конец вектора
void push_back_vector(Vector *v, const void *value) {
    if (v == NULL || value == NULL || v->CopyFunc == NULL) return;
    
    // Увеличиваем capacity при необходимости
    if (v->size >= v->capacity) {
        size_t new_capacity = (v->capacity == 0) ? 1 : v->capacity * 2;
        if (!vector_reserve(v, new_capacity)) return;
    }
    
    // Копируем и добавляем элемент
    void *dest = (char*)v->data + v->size * v->element_size;
    void *copied = v->CopyFunc(value);
    if (copied != NULL) {
        memcpy(dest, copied, v->element_size);
        // Если CopyFunc выделял память, освобождаем временный объект
        if (copied != value) { // проверяем, что это не тот же указатель
            v->DeleteFunc(copied);
        }
        v->size++;
    }
}

// Удаление элемента по индексу
void delete_at_vector(Vector *v, size_t index) {
    if (v == NULL || index >= v->size || v->DeleteFunc == NULL) return;
    
    // Удаляем элемент
    void *element = (char*)v->data + index * v->element_size;
    v->DeleteFunc(element);
    
    // Сдвигаем остальные элементы
    if (index < v->size - 1) {
        void *dest = (char*)v->data + index * v->element_size;
        void *src = (char*)v->data + (index + 1) * v->element_size;
        size_t bytes_to_move = (v->size - index - 1) * v->element_size;
        memmove(dest, src, bytes_to_move);
    }
    
    v->size--;
}

// Получение элемента по индексу
void* get_at_vector(const Vector *v, size_t index) {
    if (v == NULL || index >= v->size) {
        return NULL;
    }
    
    return (char*)v->data + index * v->element_size;
}

// Освобождение памяти, занимаемой экземпляром Vector
void delete_vector(Vector *v) {
    if (v == NULL) return;
    
    erase_vector(v);
    free(v);
}

// Получение размера вектора
size_t vector_size(const Vector *v) {
    return v != NULL ? v->size : 0;
}

// Получение вместимости вектора
size_t vector_capacity(const Vector *v) {
    return v != NULL ? v->capacity : 0;
}
