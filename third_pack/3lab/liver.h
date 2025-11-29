#ifndef LIVER_H
#define LIVER_H

#include <time.h>

typedef struct {
    unsigned int id;          
    char* surname;             
    char* name;                 
    char* patronymic;         
    struct tm birth_date;     
    char gender;              
    double income;            
} Liver;

// Функции для работы с Liver
Liver* create_liver(unsigned int id, const char* surname, const char* name, 
                   const char* patronymic, struct tm birth_date, char gender, double income);
Liver* copy_liver(const Liver* liver);
void delete_liver(Liver* liver);
int compare_liver_age(const Liver* l1, const Liver* l2);
int validate_liver(const Liver* liver);
void print_liver(const Liver* liver);

#endif
