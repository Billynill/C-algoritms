#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// БАЗОВЫЕ ОПЕРАЦИИ 

LinkedList create_list(void) {
    LinkedList list = {NULL, NULL, 0};
    return list;
}

void erase_list(LinkedList* list) {
    if (list == NULL) return;
    
    Node* current = list->head;
    while (current != NULL) {
        Node* next = current->next;
        delete_liver(current->data);
        free(current);
        current = next;
    }
    
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void delete_list(LinkedList* list) {
    erase_list(list);
}

void push_back_list(LinkedList* list, Liver* value) {
    if (list == NULL || value == NULL) return;
    
    Node* new_node = malloc(sizeof(Node));
    if (new_node == NULL) return;
    
    new_node->data = value;
    new_node->next = NULL;
    new_node->prev = list->tail;
    
    if (list->tail != NULL) {
        list->tail->next = new_node;
    }
    list->tail = new_node;
    
    if (list->head == NULL) {
        list->head = new_node;
    }
    
    list->size++;
}

void push_front_list(LinkedList* list, Liver* value) {
    if (list == NULL || value == NULL) return;
    
    Node* new_node = malloc(sizeof(Node));
    if (new_node == NULL) return;
    
    new_node->data = value;
    new_node->prev = NULL;
    new_node->next = list->head;
    
    if (list->head != NULL) {
        list->head->prev = new_node;
    }
    list->head = new_node;
    
    if (list->tail == NULL) {
        list->tail = new_node;
    }
    
    list->size++;
}

Liver* pop_back_list(LinkedList* list) {
    if (list == NULL || list->tail == NULL) return NULL;
    
    Node* last = list->tail;
    Liver* data = last->data;
    
    if (last->prev != NULL) {
        last->prev->next = NULL;
    }
    list->tail = last->prev;
    
    if (list->head == last) {
        list->head = NULL;
    }
    
    free(last);
    list->size--;
    
    return data;
}

Liver* pop_front_list(LinkedList* list) {
    if (list == NULL || list->head == NULL) return NULL;
    
    Node* first = list->head;
    Liver* data = first->data;
    
    if (first->next != NULL) {
        first->next->prev = NULL;
    }
    list->head = first->next;
    
    if (list->tail == first) {
        list->tail = NULL;
    }
    
    free(first);
    list->size--;
    
    return data;
}

void insert_at_list(LinkedList* list, size_t index, Liver* value) {
    if (list == NULL || value == NULL || index > list->size) return;
    
    if (index == 0) {
        push_front_list(list, value);
        return;
    }
    
    if (index == list->size) {
        push_back_list(list, value);
        return;
    }
    
    // Находим узел для вставки
    Node* current = list->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    
    Node* new_node = malloc(sizeof(Node));
    if (new_node == NULL) return;
    
    new_node->data = value;
    new_node->prev = current->prev;
    new_node->next = current;
    
    current->prev->next = new_node;
    current->prev = new_node;
    
    list->size++;
}

void delete_at_list(LinkedList* list, size_t index) {
    if (list == NULL || index >= list->size) return;
    
    if (index == 0) {
        pop_front_list(list);
        return;
    }
    
    if (index == list->size - 1) {
        pop_back_list(list);
        return;
    }
    
    // Находим узел для удаления
    Node* current = list->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    
    current->prev->next = current->next;
    current->next->prev = current->prev;
    
    delete_liver(current->data);
    free(current);
    list->size--;
}

Liver* get_at_list(const LinkedList* list, size_t index) {
    if (list == NULL || index >= list->size) return NULL;
    
    Node* current = list->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    
    return current->data;
}

int is_equal_list(const LinkedList* l1, const LinkedList* l2) {
    if (l1 == NULL || l2 == NULL) return 0;
    if (l1->size != l2->size) return 0;
    
    Node* node1 = l1->head;
    Node* node2 = l2->head;
    
    while (node1 != NULL && node2 != NULL) {
        if (node1->data->id != node2->data->id ||
            strcmp(node1->data->surname, node2->data->surname) != 0 ||
            strcmp(node1->data->name, node2->data->name) != 0) {
            return 0;
        }
        node1 = node1->next;
        node2 = node2->next;
    }
    
    return 1;
}

// СПЕЦИАЛЬНЫЕ ОПЕРАЦИИ ДЛЯ ЗАДАЧИ

void insert_sorted_by_age(LinkedList* list, Liver* liver) {
    if (list == NULL || liver == NULL) return;
    
    // Если список пустой или новый элемент самый старый
    if (list->size == 0 || compare_liver_age(liver, list->head->data) <= 0) {
        push_front_list(list, liver);
        return;
    }
    
    // Если новый элемент самый молодой
    if (compare_liver_age(liver, list->tail->data) >= 0) {
        push_back_list(list, liver);
        return;
    }
    
    // Ищем место для вставки
    Node* current = list->head;
    size_t index = 0;
    
    while (current != NULL && compare_liver_age(liver, current->data) > 0) {
        current = current->next;
        index++;
    }
    
    insert_at_list(list, index, liver);
}


// ФАЙЛОВЫЕ ОПЕРАЦИИ 

int load_from_file(LinkedList* list, const char* filename) {
    if (list == NULL || filename == NULL) return 0;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка: невозможно открыть файл %s\n", filename);
        return 0;
    }
    
    unsigned int id;
    char surname[100], name[100], patronymic[100];
    int day, month, year;
    char gender;
    double income;
    int line = 0;
    
    while (fscanf(file, "%u %99s %99s %99s %d.%d.%d %c %lf", 
                  &id, surname, name, patronymic, &day, &month, &year, &gender, &income) == 9) {
        line++;
        
        // Валидация данных
        if (id == 0) {
            printf("Ошибка в строке %d: неверный ID\n", line);
            continue;
        }
        
        struct tm birth_date = {0};
        birth_date.tm_mday = day;
        birth_date.tm_mon = month - 1;
        birth_date.tm_year = year - 1900;
        
        Liver* liver = create_liver(id, surname, name, patronymic, birth_date, gender, income);
        if (liver == NULL) {
            printf("Ошибка в строке %d: неверные данные жителя\n", line);
            continue;
        }
        
        if (!validate_liver(liver)) {
            printf("Ошибка в строке %d: данные не прошли валидацию\n", line);
            delete_liver(liver);
            continue;
        }
        
        insert_sorted_by_age(list, liver);
    }
    
    fclose(file);
    printf("Загружено %zu жителей из файла %s\n", list->size, filename);
    return 1;
}

int save_to_file(const LinkedList* list, const char* filename) {
    if (list == NULL || filename == NULL) return 0;
    
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка: невозможно создать файл %s\n", filename);
        return 0;
    }
    
    Node* current = list->head;
    while (current != NULL) {
        Liver* liver = current->data;
        fprintf(file, "%u %s %s %s %02d.%02d.%d %c %.2f\n",
                liver->id, liver->surname, liver->name,
                liver->patronymic ? liver->patronymic : "",
                liver->birth_date.tm_mday, liver->birth_date.tm_mon + 1,
                liver->birth_date.tm_year + 1900, liver->gender, liver->income);
        current = current->next;
    }
    
    fclose(file);
    printf("Сохранено %zu жителей в файл %s\n", list->size, filename);
    return 1;
}

//ПОИСК И МОДИФИКАЦИЯ

Liver* find_liver(const LinkedList* list, unsigned int id, const char* surname) {
    if (list == NULL) return NULL;
    
    Node* current = list->head;
    while (current != NULL) {
        Liver* liver = current->data;
        if (liver->id == id && strcmp(liver->surname, surname) == 0) {
            return liver;
        }
        current = current->next;
    }
    
    return NULL;
}

int modify_liver(LinkedList* list, unsigned int id, const Liver* new_data) {
    if (list == NULL || new_data == NULL) return 0;
    
    // Находим жителя для модификации
    Node* current = list->head;
    size_t index = 0;
    while (current != NULL) {
        if (current->data->id == id) {
            // Сохраняем предыдущее состояние для undo
            Liver* old_liver = copy_liver(current->data);
            
            // Удаляем старые данные и создаем новые
            delete_liver(current->data);
            current->data = copy_liver(new_data);
            
            // Если изменился возраст, нужно переупорядочить список
            if (compare_liver_age(old_liver, new_data) != 0) {
                Liver* temp = current->data;
                delete_at_list(list, index);
                insert_sorted_by_age(list, temp);
            }
            
            delete_liver(old_liver);
            return 1;
        }
        current = current->next;
        index++;
    }
    
    return 0;
}

void print_list(const LinkedList* list) {
    if (list == NULL) return;
    
    printf("=== Список жителей (всего: %zu) ===\n", list->size);
    Node* current = list->head;
    size_t index = 0;
    
    while (current != NULL) {
        printf("[%zu] ", index);
        print_liver(current->data);
        current = current->next;
        index++;
    }
    printf("=== Конец списка ===\n");
}

// СТЕК

void push_stack(LinkedList* stack, Liver* value) {
    push_back_list(stack, value);
}

Liver* pop_stack(LinkedList* stack) {
    return pop_back_list(stack);
}

Liver* peek_stack(const LinkedList* stack) {
    if (stack == NULL || stack->tail == NULL) return NULL;
    return stack->tail->data;
}

//ОЧЕРЕДЬ

void enqueue(LinkedList* queue, Liver* value) {
    push_back_list(queue, value);
}

Liver* dequeue(LinkedList* queue) {
    return pop_front_list(queue);
}

Liver* peek_queue(const LinkedList* queue) {
    if (queue == NULL || queue->head == NULL) return NULL;
    return queue->head->data;
}

// UNDO SYSTEM 

OperationStack create_operation_stack(size_t capacity) {
    OperationStack stack = {NULL, 0, 0};
    stack.operations = malloc(capacity * sizeof(Operation));
    if (stack.operations != NULL) {
        stack.capacity = capacity;
    }
    return stack;
}

void delete_operation_stack(OperationStack* stack) {
    if (stack == NULL) return;
    
    for (size_t i = 0; i < stack->size; i++) {
        if (stack->operations[i].previous_state != NULL) {
            delete_liver(stack->operations[i].previous_state);
        }
    }
    free(stack->operations);
    stack->operations = NULL;
    stack->size = 0;
    stack->capacity = 0;
}

void push_operation(OperationStack* stack, Operation operation) {
    if (stack == NULL) return;
    
    if (stack->size >= stack->capacity) {
        // Увеличиваем capacity при необходимости
        size_t new_capacity = stack->capacity == 0 ? 1 : stack->capacity * 2;
        Operation* new_ops = realloc(stack->operations, new_capacity * sizeof(Operation));
        if (new_ops == NULL) return;
        
        stack->operations = new_ops;
        stack->capacity = new_capacity;
    }
    
    stack->operations[stack->size++] = operation;
}

int undo_last_operations(OperationStack* stack, LinkedList* list, size_t n) {
    if (stack == NULL || list == NULL || n == 0) return 0;
    
    if (n > stack->size) {
        n = stack->size;
    }
    
    size_t undo_count = n / 2;  // N/2 операций
    if (undo_count == 0) undo_count = 1;
    
    printf("Отмена последних %zu операций из %zu...\n", undo_count, stack->size);
    
    for (size_t i = 0; i < undo_count; i++) {
        if (stack->size == 0) break;
        
        Operation op = stack->operations[--stack->size];
        
        switch (op.type) {
            case OP_ADD:
                // Отмена добавления - удаляем элемент
                if (op.liver != NULL) {
                    // Находим и удаляем жителя
                    Node* current = list->head;
                    size_t index = 0;
                    while (current != NULL) {
                        if (current->data == op.liver) {
                            delete_at_list(list, index);
                            break;
                        }
                        current = current->next;
                        index++;
                    }
                }
                break;
                
            case OP_DELETE:
                // Отмена удаления - восстанавливаем элемент
                if (op.liver != NULL) {
                    insert_sorted_by_age(list, op.liver);
                }
                break;
                
            case OP_MODIFY:
                // Отмена модификации - восстанавливаем предыдущее состояние
                if (op.liver != NULL && op.previous_state != NULL) {
                    Node* current = list->head;
                    while (current != NULL) {
                        if (current->data == op.liver) {
                            delete_liver(current->data);
                            current->data = copy_liver(op.previous_state);
                            break;
                        }
                        current = current->next;
                    }
                }
                break;
        }
        
        // Освобождаем ресурсы операции
        if (op.type == OP_DELETE || op.type == OP_MODIFY) {
            if (op.previous_state != NULL) {
                delete_liver(op.previous_state);
            }
        }
    }
    
    return 1;
}
