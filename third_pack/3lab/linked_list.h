#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "liver.h"
#include <stddef.h>

// Узел двусвязного списка
typedef struct Node {
    Liver* data;
    struct Node* prev;
    struct Node* next;
} Node;

// Двусвязный список
typedef struct {
    Node* head;
    Node* tail;
    size_t size;
} LinkedList;

// Операция для отмены (undo)
typedef enum {
    OP_ADD,
    OP_DELETE,
    OP_MODIFY
} OperationType;

typedef struct {
    OperationType type;
    Liver* liver;           // затронутый житель
    size_t index;           // индекс операции
    Liver* previous_state;  // предыдущее состояние для модификации
} Operation;

// Стек операций для undo
typedef struct {
    Operation* operations;
    size_t size;
    size_t capacity;
} OperationStack;

// БАЗОВЫЕ ОПЕРАЦИИ
LinkedList create_list(void);
void erase_list(LinkedList* list);
void delete_list(LinkedList* list);
void push_back_list(LinkedList* list, Liver* value);
void push_front_list(LinkedList* list, Liver* value);
Liver* pop_back_list(LinkedList* list);
Liver* pop_front_list(LinkedList* list);
void insert_at_list(LinkedList* list, size_t index, Liver* value);
void delete_at_list(LinkedList* list, size_t index);
Liver* get_at_list(const LinkedList* list, size_t index);
int is_equal_list(const LinkedList* l1, const LinkedList* l2);

// СПЕЦИАЛЬНЫЕ ОПЕРАЦИИ ДЛЯ ЗАДАЧИ
void insert_sorted_by_age(LinkedList* list, Liver* liver);
int load_from_file(LinkedList* list, const char* filename);
int save_to_file(const LinkedList* list, const char* filename);
Liver* find_liver(const LinkedList* list, unsigned int id, const char* surname);
int modify_liver(LinkedList* list, unsigned int id, const Liver* new_data);
void print_list(const LinkedList* list);

// СТЕК
void push_stack(LinkedList* stack, Liver* value);
Liver* pop_stack(LinkedList* stack);
Liver* peek_stack(const LinkedList* stack);

// ОЧЕРЕДЬ 
void enqueue(LinkedList* queue, Liver* value);
Liver* dequeue(LinkedList* queue);
Liver* peek_queue(const LinkedList* queue);

// UNDO SYSTEM
OperationStack create_operation_stack(size_t capacity);
void delete_operation_stack(OperationStack* stack);
void push_operation(OperationStack* stack, Operation operation);
int undo_last_operations(OperationStack* stack, LinkedList* list, size_t n);

#endif
