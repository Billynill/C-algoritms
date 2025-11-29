#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

// Функции для работы с int
void* copy_int(const void *value) {
    if (value == NULL) return NULL;
    
    // Для простых типов возвращаем тот же указатель
    // так как мы будем использовать memcpy
    return (void*)value;
}

void delete_int(void *value) {
    // Для простых типов не нужно освобождать память
    // так как данные хранятся непосредственно в векторе
    (void)value; // подавляем предупреждение о неиспользуемой переменной
}

int compare_int(const void *a, const void *b) {
    int int_a = *(const int*)a;
    int int_b = *(const int*)b;
    if (int_a < int_b) return -1;
    if (int_a > int_b) return 1;
    return 0;
}

void print_vector(const Vector *v) {
    if (v == NULL) {
        printf("Vector: NULL\n");
        return;
    }
    
    printf("Vector (size: %zu, capacity: %zu): [", v->size, v->capacity);
    for (size_t i = 0; i < v->size; i++) {
        int *value = (int*)get_at_vector(v, i);
        if (value != NULL) {
            printf("%d", *value);
            if (i < v->size - 1) printf(", ");
        }
    }
    printf("]\n");
}

int main(void) {
    printf("=== Тестирование вектора с int ===\n\n");
    
    // Создание вектора
    Vector vec = create_vector(2, sizeof(int), copy_int, delete_int);
    printf("Создан вектор: ");
    print_vector(&vec);
    
    // Добавление элементов
    printf("\n--- Добавление элементов ---\n");
    for (int i = 1; i <= 5; i++) {
        int value = i * 10;
        push_back_vector(&vec, &value);
        printf("После добавления %d: ", value);
        print_vector(&vec);
    }
    
    // Получение элементов
    printf("\n--- Получение элементов ---\n");
    for (size_t i = 0; i < vec.size; i++) {
        int *value = (int*)get_at_vector(&vec, i);
        if (value != NULL) {
            printf("Элемент [%zu] = %d\n", i, *value);
        }
    }
    
    // Копирование вектора
    printf("\n--- Копирование вектора ---\n");
    Vector vec_copy = create_vector(0, sizeof(int), copy_int, delete_int);
    copy_vector(&vec_copy, &vec);
    printf("Оригинал: ");
    print_vector(&vec);
    printf("Копия:    ");
    print_vector(&vec_copy);
    
    // Сравнение векторов
    printf("\n--- Сравнение векторов ---\n");
    printf("Оригинал и копия равны: %d\n", is_equal_vector(&vec, &vec_copy, compare_int));
    
    // Удаление элементов
    printf("\n--- Удаление элементов ---\n");
    delete_at_vector(&vec, 1); // Удаляем второй элемент
    printf("После удаления элемента [1]: ");
    print_vector(&vec);
    
    // Проверяем, что векторы теперь разные
    printf("Оригинал и копия равны после удаления: %d\n", is_equal_vector(&vec, &vec_copy, compare_int));
    
    // Копирование в динамическую память
    printf("\n--- Копирование в динамическую память ---\n");
    Vector *dynamic_vec = copy_vector_new(&vec_copy);
    if (dynamic_vec != NULL) {
        printf("Динамический вектор: ");
        print_vector(dynamic_vec);
        delete_vector(dynamic_vec);
    }
    
    // Очистка
    printf("\n--- Очистка ---\n");
    erase_vector(&vec);
    printf("После очистки оригинала: ");
    print_vector(&vec);
    
    erase_vector(&vec_copy);
    printf("После очистки копии: ");
    print_vector(&vec_copy);
    
    return 0;
}
