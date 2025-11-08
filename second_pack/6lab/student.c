#include "student.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

static int is_latin_string(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i])) return 0;
    }
    return 1;
}

Student* read_students_from_file(const char *filename, int *count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка: не удалось открыть файл '%s'\n", filename);
        return NULL;
    }
    
    *count = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        (*count)++;
    }
    
    if (*count == 0) {
        fclose(file);
        printf("Файл пуст\n");
        return NULL;
    }
    
    rewind(file);
    
    Student *students = (Student*)malloc(*count * sizeof(Student));
    if (!students) {
        fclose(file);
        printf("Ошибка выделения памяти\n");
        return NULL;
    }
    
    for (int i = 0; i < *count; i++) {
        if (!fgets(buffer, sizeof(buffer), file)) {
            break;
        }
        
        Student *student = &students[i];
        
        int parsed = sscanf(buffer, "%u %49s %49s %19s",
                           &student->id,
                           student->first_name,
                           student->last_name,
                           student->group);
        
        if (parsed != 4) {
            printf("Ошибка формата в строке %d\n", i + 1);
            continue;
        }
        
        if (!is_latin_string(student->first_name) || !is_latin_string(student->last_name)) {
            printf("Ошибка: имя или фамилия содержат нелатинские символы в строке %d\n", i + 1);
        }
        
        student->grades = (unsigned char*)malloc(EXAMS_COUNT * sizeof(unsigned char));
        if (!student->grades) {
            printf("Ошибка выделения памяти для оценок\n");
            continue;
        }
        
        char *ptr = buffer;

        for (int j = 0; j < 4; j++) {
            while (*ptr && !isspace(*ptr)) ptr++;
            while (*ptr && isspace(*ptr)) ptr++;
        }
        
        for (int j = 0; j < EXAMS_COUNT; j++) {
            if (sscanf(ptr, "%hhu", &student->grades[j]) != 1) {
                student->grades[j] = 0;
            }
            while (*ptr && !isspace(*ptr)) ptr++;
            while (*ptr && isspace(*ptr)) ptr++;
        }
    }
    
    fclose(file);
    printf("Загружено %d студентов\n", *count);
    return students;
}

void free_students(Student *students, int count) {
    if (!students) return;
    
    for (int i = 0; i < count; i++) {
        free(students[i].grades);
    }
    free(students);
}

Student* find_by_id(Student *students, int count, unsigned int id, int *found_count) {
    *found_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            (*found_count)++;
        }
    }
    
    if (*found_count == 0) return NULL;
    
    Student *found = (Student*)malloc(*found_count * sizeof(Student));
    if (!found) return NULL;
    
    int index = 0;
    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            found[index] = students[i];
            // Копируем оценки
            found[index].grades = (unsigned char*)malloc(EXAMS_COUNT * sizeof(unsigned char));
            memcpy(found[index].grades, students[i].grades, EXAMS_COUNT * sizeof(unsigned char));
            index++;
        }
    }
    
    return found;
}

Student* find_by_last_name(Student *students, int count, const char *last_name, int *found_count) {
    *found_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].last_name, last_name) == 0) {
            (*found_count)++;
        }
    }
    
    if (*found_count == 0) return NULL;
    
    Student *found = (Student*)malloc(*found_count * sizeof(Student));
    if (!found) return NULL;
    
    int index = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].last_name, last_name) == 0) {
            found[index] = students[i];
            found[index].grades = (unsigned char*)malloc(EXAMS_COUNT * sizeof(unsigned char));
            memcpy(found[index].grades, students[i].grades, EXAMS_COUNT * sizeof(unsigned char));
            index++;
        }
    }
    
    return found;
}

Student* find_by_first_name(Student *students, int count, const char *first_name, int *found_count) {
    *found_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].first_name, first_name) == 0) {
            (*found_count)++;
        }
    }
    
    if (*found_count == 0) return NULL;
    
    Student *found = (Student*)malloc(*found_count * sizeof(Student));
    if (!found) return NULL;
    
    int index = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].first_name, first_name) == 0) {
            found[index] = students[i];
            found[index].grades = (unsigned char*)malloc(EXAMS_COUNT * sizeof(unsigned char));
            memcpy(found[index].grades, students[i].grades, EXAMS_COUNT * sizeof(unsigned char));
            index++;
        }
    }
    
    return found;
}

Student* find_by_group(Student *students, int count, const char *group, int *found_count) {
    *found_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].group, group) == 0) {
            (*found_count)++;
        }
    }
    
    if (*found_count == 0) return NULL;
    
    Student *found = (Student*)malloc(*found_count * sizeof(Student));
    if (!found) return NULL;
    
    int index = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].group, group) == 0) {
            found[index] = students[i];
            found[index].grades = (unsigned char*)malloc(EXAMS_COUNT * sizeof(unsigned char));
            memcpy(found[index].grades, students[i].grades, EXAMS_COUNT * sizeof(unsigned char));
            index++;
        }
    }
    
    return found;
}

int compare_by_id(const void *a, const void *b) {
    const Student *sa = (const Student*)a;
    const Student *sb = (const Student*)b;
    return (sa->id > sb->id) - (sa->id < sb->id);
}

int compare_by_last_name(const void *a, const void *b) {
    const Student *sa = (const Student*)a;
    const Student *sb = (const Student*)b;
    return strcmp(sa->last_name, sb->last_name);
}

int compare_by_first_name(const void *a, const void *b) {
    const Student *sa = (const Student*)a;
    const Student *sb = (const Student*)b;
    return strcmp(sa->first_name, sb->first_name);
}

int compare_by_group(const void *a, const void *b) {
    const Student *sa = (const Student*)a;
    const Student *sb = (const Student*)b;
    return strcmp(sa->group, sb->group);
}

double calculate_average_grade(const Student *student) {
    if (!student || !student->grades) return 0.0;
    
    double sum = 0;
    for (int i = 0; i < EXAMS_COUNT; i++) {
        sum += student->grades[i];
    }
    return sum / EXAMS_COUNT;
}

double calculate_overall_average(Student *students, int count) {
    if (count == 0) return 0.0;
    
    double total_sum = 0;
    for (int i = 0; i < count; i++) {
        total_sum += calculate_average_grade(&students[i]);
    }
    return total_sum / count;
}

void print_student(const Student *student) {
    if (!student) return;
    
    printf("ID: %u, Фамилия: %s, Имя: %s, Группа: %s, Оценки: ",
           student->id, student->last_name, student->first_name, student->group);
    
    if (student->grades) {
        for (int i = 0; i < EXAMS_COUNT; i++) {
            printf("%u ", student->grades[i]);
        }
        printf(", Средний балл: %.2f", calculate_average_grade(student));
    }
    printf("\n");
}

void print_student_to_file(const Student *student, FILE *file) {
    if (!student || !file) return;
    
    fprintf(file, "ФИО: %s %s, Группа: %s, Средний балл: %.2f\n",
            student->last_name, student->first_name, student->group,
            calculate_average_grade(student));
}

void print_students_above_average(Student *students, int count, FILE *file) {
    if (!students || count == 0 || !file) return;
    
    double overall_avg = calculate_overall_average(students, count);
    fprintf(file, "Средний балл по всем студентам: %.2f\n", overall_avg);
    fprintf(file, "Студенты с баллом выше среднего:\n");
    
    int found = 0;
    for (int i = 0; i < count; i++) {
        double avg = calculate_average_grade(&students[i]);
        if (avg > overall_avg) {
            fprintf(file, "%s %s (%.2f)\n", students[i].last_name, students[i].first_name, avg);
            found++;
        }
    }
    
    if (found == 0) {
        fprintf(file, "Нет студентов с баллом выше среднего\n");
    }
}