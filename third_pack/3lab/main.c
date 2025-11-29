#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "linked_list.h"

void demo_basic_operations(void);
void demo_file_operations(void);
void demo_undo_system(void);

int main(void) {
    printf("=== Тестирование двусвязного списка для жителей ===\n\n");
    
    demo_basic_operations();
    demo_file_operations();
    demo_undo_system();
    
    return 0;
}

void demo_basic_operations(void) {
    printf("--- Базовые операции со списком ---\n");
    
    LinkedList list = create_list();
    
    // Создаем тестовых жителей
    struct tm birth_date1 = {0}; birth_date1.tm_mday = 15; birth_date1.tm_mon = 5; birth_date1.tm_year = 1980 - 1900;
    struct tm birth_date2 = {0}; birth_date2.tm_mday = 20; birth_date2.tm_mon = 8; birth_date2.tm_year = 1975 - 1900;
    struct tm birth_date3 = {0}; birth_date3.tm_mday = 10; birth_date3.tm_mon = 2; birth_date3.tm_year = 1990 - 1900;
    
    Liver* liver1 = create_liver(1, "Smith", "John", "Michael", birth_date1, 'M', 2500.50);
    Liver* liver2 = create_liver(2, "Johnson", "Alice", "Marie", birth_date2, 'W', 3000.75);
    Liver* liver3 = create_liver(3, "Brown", "Robert", "James", birth_date3, 'M', 2000.25);
    
    // Добавляем в отсортированном порядке по возрасту
    insert_sorted_by_age(&list, liver1);
    insert_sorted_by_age(&list, liver2);
    insert_sorted_by_age(&list, liver3);
    
    print_list(&list);
    
    // Тестирование поиска
    printf("--- Поиск жителя ---\n");
    Liver* found = find_liver(&list, 2, "Johnson");
    if (found != NULL) {
        printf("Найден: ");
        print_liver(found);
    }
    
    // Тестирование модификации
    printf("--- Модификация жителя ---\n");
    struct tm new_birth_date = {0}; new_birth_date.tm_mday = 25; new_birth_date.tm_mon = 11; new_birth_date.tm_year = 1985 - 1900;
    Liver* new_data = create_liver(2, "Johnson", "Alice", "Marie", new_birth_date, 'W', 3500.00);
    if (modify_liver(&list, 2, new_data)) {
        printf("Житель изменен. Новый список:\n");
        print_list(&list);
    }
    delete_liver(new_data);
    
    // Очистка
    delete_list(&list);
    printf("\n");
}

void demo_file_operations(void) {
    printf("--- Работа с файлами ---\n");
    
    // Создаем тестовый файл
    FILE* test_file = fopen("test_livers.txt", "w");
    if (test_file != NULL) {
        fprintf(test_file, "1 Smith John Michael 15.06.1980 M 2500.50\n");
        fprintf(test_file, "2 Johnson Alice Marie 20.09.1975 W 3000.75\n");
        fprintf(test_file, "3 Brown Robert James 10.03.1990 M 2000.25\n");
        fclose(test_file);
    }
    
    LinkedList list = create_list();
    
    if (load_from_file(&list, "test_livers.txt")) {
        print_list(&list);
        
        // Сохраняем в другой файл
        save_to_file(&list, "output_livers.txt");
    }
    
    delete_list(&list);
    printf("\n");
}

void demo_undo_system(void) {
    printf("--- Система отмены операций ---\n");
    
    LinkedList list = create_list();
    OperationStack undo_stack = create_operation_stack(10);
    
    // Добавляем жителей с записью операций
    struct tm birth_date1 = {0}; birth_date1.tm_mday = 15; birth_date1.tm_mon = 5; birth_date1.tm_year = 1980 - 1900;
    struct tm birth_date2 = {0}; birth_date2.tm_mday = 20; birth_date2.tm_mon = 8; birth_date2.tm_year = 1975 - 1900;
    
    Liver* liver1 = create_liver(1, "Smith", "John", "Michael", birth_date1, 'M', 2500.50);
    Liver* liver2 = create_liver(2, "Johnson", "Alice", "Marie", birth_date2, 'W', 3000.75);
    
    insert_sorted_by_age(&list, liver1);
    push_operation(&undo_stack, (Operation){OP_ADD, liver1, 0, NULL});
    
    insert_sorted_by_age(&list, liver2);
    push_operation(&undo_stack, (Operation){OP_ADD, liver2, 1, NULL});
    
    printf("После добавления 2 жителей:\n");
    print_list(&list);
    
    // Отменяем операции
    undo_last_operations(&undo_stack, &list, 4); // Отменяем N/2 = 2/2 = 1 операцию
    
    printf("После отмены:\n");
    print_list(&list);
    
    delete_list(&list);
    delete_operation_stack(&undo_stack);
}
