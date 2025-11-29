#ifndef POSTAL_SERVICE_H
#define POSTAL_SERVICE_H

#include "heap.h"
#include <time.h>
#include <stdio.h>

#define MAX_CONNECTIONS 10
#define MAX_TECH_DATA 256

typedef enum {
    LETTER_REGULAR,
    LETTER_URGENT,
    LETTER_REGISTERED
} LetterType;

typedef enum {
    STATUS_IN_TRANSIT,
    STATUS_DELIVERED,
    STATUS_UNDELIVERED,
    STATUS_IN_OFFICE
} LetterStatus;

typedef struct {
    unsigned int id;
    LetterType type;
    int priority;
    unsigned int from_office_id;
    unsigned int to_office_id;
    LetterStatus status;
    char tech_data[MAX_TECH_DATA];
    time_t created_time;
} Letter;

typedef struct PostalOffice PostalOffice;

struct PostalOffice {
    unsigned int id;
    size_t capacity;
    size_t current_count;
    Heap letter_queue;  // косая приоритетная очередь для писем
    unsigned int connections[MAX_CONNECTIONS];
    size_t connections_count;
    PostalOffice *next;
};

typedef struct {
    PostalOffice *offices;
    Letter *letters;
    size_t letters_count;
    size_t letters_capacity;
    unsigned int next_letter_id;
    FILE *log_file;
} PostalSystem;

// Системные функции
PostalSystem create_postal_system(const char *log_filename);
void delete_postal_system(PostalSystem *system);
int load_mappings(PostalSystem *system, const char *filename);

// Функции почтовых отделений
int add_post_office(PostalSystem *system, unsigned int id, size_t capacity, 
                   const unsigned int *connections, size_t conn_count);
int remove_post_office(PostalSystem *system, unsigned int id);
PostalOffice* find_office(PostalSystem *system, unsigned int id);

// Функции писем
unsigned int add_letter(PostalSystem *system, LetterType type, int priority,
                       unsigned int from_office, unsigned int to_office, 
                       const char *tech_data);
int mark_letter_undelivered(PostalSystem *system, unsigned int letter_id);
int pickup_letter(PostalSystem *system, unsigned int letter_id, unsigned int office_id);
int get_all_letters(const PostalSystem *system, const char *filename);

// Симуляция
void simulate_mail_delivery(PostalSystem *system);
void process_office_mail(PostalSystem *system, PostalOffice *office);

// Утилиты
void log_message(PostalSystem *system, const char *message);
void print_system_state(const PostalSystem *system);

#endif
