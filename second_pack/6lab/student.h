#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>
#include <stddef.h>

#define MAX_NAME_LENGTH 50
#define MAX_GROUP_LENGTH 20
#define EXAMS_COUNT 5

typedef struct {
    unsigned int id;
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char group[MAX_GROUP_LENGTH];
    unsigned char *grades; 
} Student;

Student* read_students_from_file(const char *filename, int *count);
void free_students(Student *students, int count);

Student* find_by_id(Student *students, int count, unsigned int id, int *found_count);
Student* find_by_last_name(Student *students, int count, const char *last_name, int *found_count);
Student* find_by_first_name(Student *students, int count, const char *first_name, int *found_count);
Student* find_by_group(Student *students, int count, const char *group, int *found_count);

int compare_by_id(const void *a, const void *b);
int compare_by_last_name(const void *a, const void *b);
int compare_by_first_name(const void *a, const void *b);
int compare_by_group(const void *a, const void *b);

double calculate_average_grade(const Student *student);
double calculate_overall_average(Student *students, int count);
void print_student(const Student *student);
void print_student_to_file(const Student *student, FILE *file);
void print_students_above_average(Student *students, int count, FILE *file);

#endif