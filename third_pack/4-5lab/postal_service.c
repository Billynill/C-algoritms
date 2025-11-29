#include "postal_service.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

PostalSystem create_postal_system(const char *log_filename) {
    PostalSystem system = {NULL, NULL, 0, 0, 1, NULL};
    
    if (log_filename != NULL) {
        system.log_file = fopen(log_filename, "w");
        if (system.log_file == NULL) {
            printf("Ошибка: невозможно создать файл логов %s\n", log_filename);
        }
    }
    
    log_message(&system, "Почтовая система инициализирована");
    return system;
}

void delete_postal_system(PostalSystem *system) {
    if (system == NULL) return;
    
    log_message(system, "Завершение работы почтовой системы");
    
    // Удаляем все отделения
    PostalOffice *current = system->offices;
    while (current != NULL) {
        PostalOffice *next = current->next;
        delete_heap(&current->letter_queue);
        free(current);
        current = next;
    }
    
    // Удаляем все письма
    free(system->letters);
    
    if (system->log_file != NULL) {
        fclose(system->log_file);
    }
    
    system->offices = NULL;
    system->letters = NULL;
    system->letters_count = 0;
    system->letters_capacity = 0;
}

int load_mappings(PostalSystem *system, const char *filename) {
    if (system == NULL || filename == NULL) return 0;
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        log_message(system, "Ошибка: невозможно открыть файл маппингов");
        return 0;
    }
    
    unsigned int id1, id2;
    int mapping_count = 0;
    
    while (fscanf(file, "%u %u", &id1, &id2) == 2) {
        // Создаем отделения если их нет
        if (find_office(system, id1) == NULL) {
            add_post_office(system, id1, 10, NULL, 0);
        }
        if (find_office(system, id2) == NULL) {
            add_post_office(system, id2, 10, NULL, 0);
        }
        
        // Добавляем связь
        PostalOffice *office1 = find_office(system, id1);
        PostalOffice *office2 = find_office(system, id2);
        
        if (office1 != NULL && office2 != NULL) {
            // Добавляем связь в обе стороны
            if (office1->connections_count < MAX_CONNECTIONS) {
                office1->connections[office1->connections_count++] = id2;
            }
            if (office2->connections_count < MAX_CONNECTIONS) {
                office2->connections[office2->connections_count++] = id1;
            }
            mapping_count++;
        }
    }
    
    fclose(file);
    
    char log_msg[100];
    snprintf(log_msg, sizeof(log_msg), "Загружено %d связей между отделениями", mapping_count);
    log_message(system, log_msg);
    
    return 1;
}

int add_post_office(PostalSystem *system, unsigned int id, size_t capacity, 
                   const unsigned int *connections, size_t conn_count) {
    if (system == NULL || find_office(system, id) != NULL) return 0;
    
    PostalOffice *new_office = malloc(sizeof(PostalOffice));
    if (new_office == NULL) return 0;
    
    new_office->id = id;
    new_office->capacity = capacity;
    new_office->current_count = 0;
    new_office->connections_count = 0;
    new_office->letter_queue = create_heap(10);
    
    // Копируем связи если предоставлены
    if (connections != NULL && conn_count > 0) {
        for (size_t i = 0; i < conn_count && i < MAX_CONNECTIONS; i++) {
            new_office->connections[new_office->connections_count++] = connections[i];
        }
    }
    
    // Добавляем в список
    new_office->next = system->offices;
    system->offices = new_office;
    
    char log_msg[100];
    snprintf(log_msg, sizeof(log_msg), "Добавлено почтовое отделение ID: %u, вместимость: %zu", id, capacity);
    log_message(system, log_msg);
    
    return 1;
}

int remove_post_office(PostalSystem *system, unsigned int id) {
    if (system == NULL) return 0;
    
    PostalOffice **prev = &system->offices;
    PostalOffice *current = system->offices;
    
    while (current != NULL && current->id != id) {
        prev = &current->next;
        current = current->next;
    }
    
    if (current == NULL) return 0;
    
    // Обрабатываем письма в этом отделении
    while (!is_empty_heap(&current->letter_queue)) {
        int letter_id = pop_heap(&current->letter_queue);
        mark_letter_undelivered(system, (unsigned int)letter_id);
    }
    
    // Удаляем связи с другими отделениями
    PostalOffice *other = system->offices;
    while (other != NULL) {
        for (size_t i = 0; i < other->connections_count; i++) {
            if (other->connections[i] == id) {
                // Удаляем связь
                for (size_t j = i; j < other->connections_count - 1; j++) {
                    other->connections[j] = other->connections[j + 1];
                }
                other->connections_count--;
                break;
            }
        }
        other = other->next;
    }
    
    // Удаляем отделение
    *prev = current->next;
    delete_heap(&current->letter_queue);
    free(current);
    
    char log_msg[100];
    snprintf(log_msg, sizeof(log_msg), "Удалено почтовое отделение ID: %u", id);
    log_message(system, log_msg);
    
    return 1;
}

PostalOffice* find_office(PostalSystem *system, unsigned int id) {
    if (system == NULL) return NULL;
    
    PostalOffice *current = system->offices;
    while (current != NULL) {
        if (current->id == id) return current;
        current = current->next;
    }
    
    return NULL;
}

unsigned int add_letter(PostalSystem *system, LetterType type, int priority,
                       unsigned int from_office, unsigned int to_office, 
                       const char *tech_data) {
    if (system == NULL) return 0;
    
    // Проверяем существование отделений
    if (find_office(system, from_office) == NULL || 
        find_office(system, to_office) == NULL) {
        return 0;
    }
    
    // Расширяем массив писем если нужно
    if (system->letters_count >= system->letters_capacity) {
        size_t new_capacity = system->letters_capacity == 0 ? 10 : system->letters_capacity * 2;
        Letter *new_letters = realloc(system->letters, new_capacity * sizeof(Letter));
        if (new_letters == NULL) return 0;
        
        system->letters = new_letters;
        system->letters_capacity = new_capacity;
    }
    
    // Создаем письмо
    Letter *letter = &system->letters[system->letters_count];
    letter->id = system->next_letter_id++;
    letter->type = type;
    letter->priority = priority;
    letter->from_office_id = from_office;
    letter->to_office_id = to_office;
    letter->status = STATUS_IN_OFFICE;
    letter->created_time = time(NULL);
    
    if (tech_data != NULL) {
        strncpy(letter->tech_data, tech_data, MAX_TECH_DATA - 1);
        letter->tech_data[MAX_TECH_DATA - 1] = '\0';
    } else {
        letter->tech_data[0] = '\0';
    }
    
    // Добавляем в отделение отправителя
    PostalOffice *office = find_office(system, from_office);
    if (office != NULL && office->current_count < office->capacity) {
        push_heap(&office->letter_queue, (int)letter->id);
        office->current_count++;
        system->letters_count++;
        
        char log_msg[150];
        snprintf(log_msg, sizeof(log_msg), 
                "Добавлено письмо ID: %u, тип: %d, приоритет: %d, из: %u, в: %u",
                letter->id, type, priority, from_office, to_office);
        log_message(system, log_msg);
        
        return letter->id;
    }
    
    return 0;
}

int mark_letter_undelivered(PostalSystem *system, unsigned int letter_id) {
    if (system == NULL) return 0;
    
    for (size_t i = 0; i < system->letters_count; i++) {
        if (system->letters[i].id == letter_id) {
            system->letters[i].status = STATUS_UNDELIVERED;
            
            char log_msg[100];
            snprintf(log_msg, sizeof(log_msg), 
                    "Письмо ID: %u помечено как недоставленное", letter_id);
            log_message(system, log_msg);
            return 1;
        }
    }
    
    return 0;
}

int pickup_letter(PostalSystem *system, unsigned int letter_id, unsigned int office_id) {
    if (system == NULL) return 0;
    
    for (size_t i = 0; i < system->letters_count; i++) {
        Letter *letter = &system->letters[i];
        if (letter->id == letter_id && letter->to_office_id == office_id) {
            if (letter->status == STATUS_IN_OFFICE) {
                letter->status = STATUS_DELIVERED;
                
                // Удаляем из отделения
                PostalOffice *office = find_office(system, office_id);
                if (office != NULL) {
                    // В реальной реализации нужно удалить из кучи
                    // Для простоты просто уменьшаем счетчик
                    if (office->current_count > 0) {
                        office->current_count--;
                    }
                }
                
                char log_msg[100];
                snprintf(log_msg, sizeof(log_msg), 
                        "Письмо ID: %u доставлено в отделение %u", letter_id, office_id);
                log_message(system, log_msg);
                return 1;
            }
        }
    }
    
    return 0;
}

int get_all_letters(const PostalSystem *system, const char *filename) {
    if (system == NULL || filename == NULL) return 0;
    
    FILE *file = fopen(filename, "w");
    if (file == NULL) return 0;
    
    fprintf(file, "=== Список всех писем (всего: %zu) ===\n", system->letters_count);
    
    for (size_t i = 0; i < system->letters_count; i++) {
        const Letter *letter = &system->letters[i];
        const char *status_str = "";
        const char *type_str = "";
        
        switch (letter->status) {
            case STATUS_IN_TRANSIT: status_str = "В пути"; break;
            case STATUS_DELIVERED: status_str = "Доставлено"; break;
            case STATUS_UNDELIVERED: status_str = "Не доставлено"; break;
            case STATUS_IN_OFFICE: status_str = "В отделении"; break;
        }
        
        switch (letter->type) {
            case LETTER_REGULAR: type_str = "Обычное"; break;
            case LETTER_URGENT: type_str = "Срочное"; break;
            case LETTER_REGISTERED: type_str = "Заказное"; break;
        }
        
        fprintf(file, "Письмо ID: %u\n", letter->id);
        fprintf(file, "  Тип: %s\n", type_str);
        fprintf(file, "  Приоритет: %d\n", letter->priority);
        fprintf(file, "  От: %u -> К: %u\n", letter->from_office_id, letter->to_office_id);
        fprintf(file, "  Статус: %s\n", status_str);
        fprintf(file, "  Данные: %s\n", letter->tech_data);
        fprintf(file, "  Создано: %s", ctime(&letter->created_time));
        fprintf(file, "---\n");
    }
    
    fclose(file);
    
    char log_msg[100];
    snprintf(log_msg, sizeof(log_msg), 
            "Список писем экспортирован в файл: %s", filename);
    if (system->log_file != NULL) {
        fprintf(system->log_file, "%s\n", log_msg);
        fflush(system->log_file);
    }
    
    return 1;
}

void simulate_mail_delivery(PostalSystem *system) {
    if (system == NULL) return;
    
    log_message(system, "Запуск симуляции доставки почты");
    
    PostalOffice *office = system->offices;
    while (office != NULL) {
        process_office_mail(system, office);
        office = office->next;
    }
}

void process_office_mail(PostalSystem *system, PostalOffice *office) {
    if (system == NULL || office == NULL || is_empty_heap(&office->letter_queue)) return;
    
    // Берем самое приоритетное письмо
    int letter_id = peek_heap(&office->letter_queue);
    Letter *letter = NULL;
    
    // Находим письмо в массиве
    for (size_t i = 0; i < system->letters_count; i++) {
        if (system->letters[i].id == (unsigned int)letter_id) {
            letter = &system->letters[i];
            break;
        }
    }
    
    if (letter == NULL) return;
    
    // Если письмо уже в пункте назначения
    if (office->id == letter->to_office_id) {
        letter->status = STATUS_IN_OFFICE;
        pop_heap(&office->letter_queue); // Убираем из очереди обработки
        office->current_count--;
        
        char log_msg[100];
        snprintf(log_msg, sizeof(log_msg), 
                "Письмо ID: %u достигло пункта назначения (отделение %u)", 
                letter->id, office->id);
        log_message(system, log_msg);
        return;
    }
    
    // Ищем следующее отделение для пересылки
    unsigned int next_office_id = 0;
    for (size_t i = 0; i < office->connections_count; i++) {
        PostalOffice *conn_office = find_office(system, office->connections[i]);
        if (conn_office != NULL && conn_office->current_count < conn_office->capacity) {
            next_office_id = office->connections[i];
            break;
        }
    }
    
    if (next_office_id != 0) {
        // Пересылаем письмо
        PostalOffice *next_office = find_office(system, next_office_id);
        if (next_office != NULL) {
            // Удаляем из текущего отделения
            pop_heap(&office->letter_queue);
            office->current_count--;
            
            // Добавляем в следующее отделение
            push_heap(&next_office->letter_queue, letter_id);
            next_office->current_count++;
            letter->status = STATUS_IN_TRANSIT;
            
            char log_msg[150];
            snprintf(log_msg, sizeof(log_msg), 
                    "Письмо ID: %u переслано из отделения %u в отделение %u", 
                    letter->id, office->id, next_office_id);
            log_message(system, log_msg);
        }
    } else {
        // Нет доступных соединений - помечаем как недоставленное
        mark_letter_undelivered(system, letter->id);
        pop_heap(&office->letter_queue);
        office->current_count--;
    }
}

void log_message(PostalSystem *system, const char *message) {
    if (system == NULL || message == NULL) return;
    
    time_t now = time(NULL);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    if (system->log_file != NULL) {
        fprintf(system->log_file, "[%s] %s\n", time_str, message);
        fflush(system->log_file);
    }
    
    printf("[%s] %s\n", time_str, message);
}

void print_system_state(const PostalSystem *system) {
    if (system == NULL) return;
    
    printf("\n=== ТЕКУЩЕЕ СОСТОЯНИЕ СИСТЕМЫ ===\n");
    printf("Всего писем: %zu\n", system->letters_count);
    printf("Почтовые отделения:\n");
    
    PostalOffice *office = system->offices;
    while (office != NULL) {
        printf("  Отделение ID: %u, писем: %zu/%zu, связей: %zu\n",
               office->id, office->current_count, office->capacity, 
               office->connections_count);
        office = office->next;
    }
    printf("================================\n\n");
}
