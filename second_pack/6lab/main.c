#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void create_sample_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file) {
        fprintf(file, "1 John Smith CS101 5 4 5 4 5\n");
        fprintf(file, "2 Jane Doe CS101 4 5 4 5 4\n");
        fprintf(file, "3 Bob Johnson CS102 3 4 3 4 3\n");
        fprintf(file, "4 Alice Brown CS101 5 5 5 5 5\n");
        fprintf(file, "5 Charlie Wilson CS102 4 4 4 4 4\n");
        fclose(file);
        printf("Создан тестовый файл '%s'\n", filename);
    }
}

void print_menu() {
    printf("\n=== СИСТЕМА УПРАВЛЕНИЯ СТУДЕНТАМИ ===\n");
    printf("1. Поиск студентов\n");
    printf("2. Сортировка студентов\n");
    printf("3. Вывод студента по ID в файл\n");
    printf("4. Вывод студентов с баллом выше среднего в файл\n");
    printf("5. Показать всех студентов\n");
    printf("0. Выход\n");
    printf("Выберите действие: ");
}

void search_menu(Student *students, int count) {
    int choice;
    
    printf("\n--- ПОИСК СТУДЕНТОВ ---\n");
    printf("1. По ID\n");
    printf("2. По фамилии\n");
    printf("3. По имени\n");
    printf("4. По группе\n");
    printf("0. Назад\n");
    printf("Выберите тип поиска: ");
    
    scanf("%d", &choice);
    clear_input_buffer();
    
    if (choice == 0) return;
    
    Student *found = NULL;
    int found_count = 0;
    char input[100];
    
    switch (choice) {
        case 1: {
            unsigned int id;
            printf("Введите ID: ");
            scanf("%u", &id);
            clear_input_buffer();
            found = find_by_id(students, count, id, &found_count);
            break;
        }
        case 2: {
            printf("Введите фамилию: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0';
            found = find_by_last_name(students, count, input, &found_count);
            break;
        }
        case 3: {
            printf("Введите имя: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0';
            found = find_by_first_name(students, count, input, &found_count);
            break;
        }
        case 4: {
            printf("Введите группу: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0';
            found = find_by_group(students, count, input, &found_count);
            break;
        }
        default:
            printf("Неверный выбор!\n");
            return;
    }
    
    if (found_count == 0) {
        printf("Студенты не найдены\n");
    } else {
        printf("Найдено студентов: %d\n", found_count);
        for (int i = 0; i < found_count; i++) {
            printf("%d. ", i + 1);
            print_student(&found[i]);
        }
        
        for (int i = 0; i < found_count; i++) {
            free(found[i].grades);
        }
        free(found);
    }
}

void sort_menu(Student *students, int count) {
    int choice;
    
    printf("\n--- СОРТИРОВКА СТУДЕНТОВ ---\n");
    printf("1. По ID\n");
    printf("2. По фамилии\n");
    printf("3. По имени\n");
    printf("4. По группе\n");
    printf("0. Назад\n");
    printf("Выберите тип сортировки: ");
    
    scanf("%d", &choice);
    clear_input_buffer();
    
    if (choice == 0) return;
    
    Student *sorted = (Student*)malloc(count * sizeof(Student));
    if (!sorted) {
        printf("Ошибка выделения памяти\n");
        return;
    }
    
    for (int i = 0; i < count; i++) {
        sorted[i] = students[i];
        sorted[i].grades = (unsigned char*)malloc(EXAMS_COUNT * sizeof(unsigned char));
        memcpy(sorted[i].grades, students[i].grades, EXAMS_COUNT * sizeof(unsigned char));
    }
    
    switch (choice) {
        case 1:
            qsort(sorted, count, sizeof(Student), compare_by_id);
            printf("Отсортировано по ID:\n");
            break;
        case 2:
            qsort(sorted, count, sizeof(Student), compare_by_last_name);
            printf("Отсортировано по фамилии:\n");
            break;
        case 3:
            qsort(sorted, count, sizeof(Student), compare_by_first_name);
            printf("Отсортировано по имени:\n");
            break;
        case 4:
            qsort(sorted, count, sizeof(Student), compare_by_group);
            printf("Отсортировано по группе:\n");
            break;
        default:
            printf("Неверный выбор!\n");
            for (int i = 0; i < count; i++) {
                free(sorted[i].grades);
            }
            free(sorted);
            return;
    }
    
    for (int i = 0; i < count; i++) {
        printf("%d. ", i + 1);
        print_student(&sorted[i]);
    }
    
    for (int i = 0; i < count; i++) {
        free(sorted[i].grades);
    }
    free(sorted);
}

void export_student_by_id(Student *students, int count, const char *trace_file) {
    unsigned int id;
    
    printf("Введите ID студента для экспорта: ");
    scanf("%u", &id);
    clear_input_buffer();
    
    int found_count;
    Student *found = find_by_id(students, count, id, &found_count);
    
    if (found_count == 0) {
        printf("Студент с ID %u не найден\n", id);
        return;
    }
    
    FILE *file = fopen(trace_file, "a");
    if (!file) {
        printf("Ошибка открытия файла '%s'\n", trace_file);
        for (int i = 0; i < found_count; i++) {
            free(found[i].grades);
        }
        free(found);
        return;
    }
    
    fprintf(file, "\n=== Экспорт студента по ID %u ===\n", id);
    for (int i = 0; i < found_count; i++) {
        print_student_to_file(&found[i], file);
    }
    fclose(file);
    
    printf("Данные экспортированы в файл '%s'\n", trace_file);
    
    for (int i = 0; i < found_count; i++) {
        free(found[i].grades);
    }
    free(found);
}

void export_above_average(Student *students, int count, const char *trace_file) {
    FILE *file = fopen(trace_file, "a");
    if (!file) {
        printf("Ошибка открытия файла '%s'\n", trace_file);
        return;
    }
    
    fprintf(file, "\n=== Студенты с баллом выше среднего ===\n");
    print_students_above_average(students, count, file);
    fclose(file);
    
    printf("Данные экспортированы в файл '%s'\n", trace_file);
}

void show_all_students(Student *students, int count) {
    printf("\n--- ВСЕ СТУДЕНТЫ (%d) ---\n", count);
    for (int i = 0; i < count; i++) {
        printf("%d. ", i + 1);
        print_student(&students[i]);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Использование: %s <файл_студентов> <трассировочный_файл>\n", argv[0]);
        printf("Пример: %s students.txt trace.log\n", argv[0]);
        
        printf("Создать тестовый файл? (y/n): ");
        char answer;
        scanf("%c", &answer);
        clear_input_buffer();
        
        if (answer == 'y' || answer == 'Y') {
            create_sample_file("students.txt");
            printf("Теперь запусти: %s students.txt trace.log\n", argv[0]);
        }
        return 1;
    }
    
    const char *student_file = argv[1];
    const char *trace_file = argv[2];
    
    printf("Загрузка студентов из файла: %s\n", student_file);
    printf("Трассировочный файл: %s\n", trace_file);
    
    int student_count = 0;
    Student *students = read_students_from_file(student_file, &student_count);
    
    if (!students || student_count == 0) {
        printf("Не удалось загрузить студентов. Создать тестовый файл? (y/n): ");
        char answer;
        scanf("%c", &answer);
        clear_input_buffer();
        
        if (answer == 'y' || answer == 'Y') {
            create_sample_file(student_file);
            students = read_students_from_file(student_file, &student_count);
        }
        
        if (!students || student_count == 0) {
            printf("Не удалось загрузить студентов\n");
            return 1;
        }
    }
    
    int choice;
    do {
        print_menu();
        scanf("%d", &choice);
        clear_input_buffer();
        
        switch (choice) {
            case 1:
                search_menu(students, student_count);
                break;
            case 2:
                sort_menu(students, student_count);
                break;
            case 3:
                export_student_by_id(students, student_count, trace_file);
                break;
            case 4:
                export_above_average(students, student_count, trace_file);
                break;
            case 5:
                show_all_students(students, student_count);
                break;
            case 0:
                printf("Выход...\n");
                break;
            default:
                printf("Неверный выбор!\n");
        }
        
        if (choice != 0) {
            printf("\nНажмите Enter для продолжения...");
            getchar();
        }
        
    } while (choice != 0);
    
    free_students(students, student_count);
    return 0;
}