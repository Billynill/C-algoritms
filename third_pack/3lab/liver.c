#include "liver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// Проверка строки на содержание только латинских букв
static int is_latin_string(const char* str, int allow_empty) {
    if (str == NULL) return 0;
    if (allow_empty && str[0] == '\0') return 1;
    if (str[0] == '\0') return 0;
    
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i])) return 0;
    }
    return 1;
}

// Создание нового жителя
Liver* create_liver(unsigned int id, const char* surname, const char* name, 
                   const char* patronymic, struct tm birth_date, char gender, double income) {
    
    // Валидация входных данных
    if (!is_latin_string(surname, 0) || !is_latin_string(name, 0) || 
        !is_latin_string(patronymic, 1)) {
        return NULL;
    }
    if (gender != 'M' && gender != 'W') return NULL;
    if (income < 0) return NULL;
    
    Liver* liver = malloc(sizeof(Liver));
    if (liver == NULL) return NULL;
    
    liver->id = id;
    liver->gender = gender;
    liver->income = income;
    liver->birth_date = birth_date;
    
    // Копируем строки
    liver->surname = malloc(strlen(surname) + 1);
    liver->name = malloc(strlen(name) + 1);
    
    if (liver->surname == NULL || liver->name == NULL) {
        free(liver->surname);
        free(liver->name);
        free(liver);
        return NULL;
    }
    
    strcpy(liver->surname, surname);
    strcpy(liver->name, name);
    
    if (patronymic[0] != '\0') {
        liver->patronymic = malloc(strlen(patronymic) + 1);
        if (liver->patronymic == NULL) {
            free(liver->surname);
            free(liver->name);
            free(liver);
            return NULL;
        }
        strcpy(liver->patronymic, patronymic);
    } else {
        liver->patronymic = NULL;
    }
    
    return liver;
}

// Копирование жителя
Liver* copy_liver(const Liver* liver) {
    if (liver == NULL) return NULL;
    
    return create_liver(liver->id, liver->surname, liver->name, 
                       liver->patronymic ? liver->patronymic : "", 
                       liver->birth_date, liver->gender, liver->income);
}

// Удаление жителя
void delete_liver(Liver* liver) {
    if (liver == NULL) return;
    
    free(liver->surname);
    free(liver->name);
    if (liver->patronymic != NULL) {
        free(liver->patronymic);
    }
    free(liver);
}

// Сравнение по возрасту (для упорядочивания)
int compare_liver_age(const Liver* l1, const Liver* l2) {
    if (l1 == NULL || l2 == NULL) return 0;
    
    time_t time1 = mktime((struct tm*)&l1->birth_date);
    time_t time2 = mktime((struct tm*)&l2->birth_date);
    
    if (time1 < time2) return -1;  // l1 старше
    if (time1 > time2) return 1;   // l1 моложе
    return 0;                      // одинаковый возраст
}

// Валидация жителя
int validate_liver(const Liver* liver) {
    if (liver == NULL) return 0;
    if (!is_latin_string(liver->surname, 0) || !is_latin_string(liver->name, 0) || 
        !is_latin_string(liver->patronymic, 1)) return 0;
    if (liver->gender != 'M' && liver->gender != 'W') return 0;
    if (liver->income < 0) return 0;
    
    return 1;
}

// Печать информации о жителе
void print_liver(const Liver* liver) {
    if (liver == NULL) return;
    
    printf("ID: %u, Name: %s %s %s, Birth: %02d.%02d.%d, Gender: %c, Income: %.2f\n",
           liver->id, liver->surname, liver->name, 
           liver->patronymic ? liver->patronymic : "",
           liver->birth_date.tm_mday, liver->birth_date.tm_mon + 1, 
           liver->birth_date.tm_year + 1900, liver->gender, liver->income);
}
