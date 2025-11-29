#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "postal_service.h"

void print_menu(void) {
    printf("\n=== ПОЧТОВЫЙ СЕРВИС ===\n");
    printf("1. Добавить почтовое отделение\n");
    printf("2. Удалить почтовое отделение\n");
    printf("3. Добавить письмо\n");
    printf("4. Пометить письмо как недоставленное\n");
    printf("5. Забрать письмо\n");
    printf("6. Получить список всех писем\n");
    printf("7. Запустить симуляцию доставки\n");
    printf("8. Показать состояние системы\n");
    printf("9. Выход\n");
    printf("Выберите действие: ");
}

void add_office_ui(PostalSystem *system) {
    unsigned int id;
    size_t capacity;
    
    printf("Введите ID отделения: ");
    if (scanf("%u", &id) != 1) {
        printf("Неверный ввод!\n");
        while (getchar() != '\n');
        return;
    }
    
    printf("Введите вместимость отделения: ");
    if (scanf("%zu", &capacity) != 1) {
        printf("Неверный ввод!\n");
        while (getchar() != '\n');
        return;
    }
    
    if (add_post_office(system, id, capacity, NULL, 0)) {
        printf("Отделение успешно добавлено!\n");
    } else {
        printf("Ошибка при добавлении отделения!\n");
    }
}

void remove_office_ui(PostalSystem *system) {
    unsigned int id;
    
    printf("Введите ID отделения для удаления: ");
    if (scanf("%u", &id) != 1) {
        printf("Неверный ввод!\n");
        while (getchar() != '\n');
        return;
    }
    
    if (remove_post_office(system, id)) {
        printf("Отделение успешно удалено!\n");
    } else {
        printf("Ошибка при удалении отделения!\n");
    }
}

void add_letter_ui(PostalSystem *system) {
    unsigned int from_office, to_office;
    int type, priority;
    char tech_data[256];
    
    printf("Тип письма (0-Обычное, 1-Срочное, 2-Заказное): ");
    if (scanf("%d", &type) != 1) {
        printf("Неверный ввод!\n");
        while (getchar() != '\n');
        return;
    }
    
    printf("Приоритет (чем выше число, тем выше приоритет): ");
    if (scanf("%d", &priority) != 1) {
        printf("Неверный ввод!\n");
        while (getchar() != '\n');
        return;
    }
    
    printf("ID отделения отправителя: ");
    if (scanf("%u", &from_office) != 1) {
        printf("Неверный ввод!\n");
        while (getchar() != '\n');
        return;
    }
    
    printf("ID отделения получателя: ");
    if (scanf("%u", &to_office) != 1) {
        printf("Неверный ввод!\n");
        while (getchar() != '\n');
        return;
    }
    
    printf("Технические данные: ");
    if (scanf("%255s", tech_data) != 1) {
        printf("Неверный ввод!\n");
        while (getchar() != '\n');
        return;
    }
    
    unsigned int letter_id = add_letter(system, (LetterType)type, priority, 
                                       from_office, to_office, tech_data);
    if (letter_id != 0) {
        printf("Письмо успешно добавлено! ID: %u\n", letter_id);
    } else {
        printf("Ошибка при добавлении письма!\n");
    }
}

void mark_undelivered_ui(PostalSystem *system) {
    unsigned int letter_id;
    
    printf("Введите ID письма: ");
    if (scanf("%u", &letter_id) != 1) {
        printf("Неверный ввод!\n");
        while (getchar() != '\n');
        return;
    }
    
    if (mark_letter_undelivered(system, letter_id)) {
        printf("Письмо помечено как недоставленное!\n");
    } else {
        printf("Ошибка! Письмо не найдено.\n");
    }
}

void pickup_letter_ui(PostalSystem *system) {
    unsigned int letter_id, office_id;
    
    printf("Введите ID письма: ");
    if (scanf("%u", &letter_id) != 1) {
        printf("Неверный ввод!\n");
        while (getchar() != '\n');
        return;
    }
    
    printf("Введите ID отделения: ");
    if (scanf("%u", &office_id) != 1) {
        printf("Неверный ввод!\n");
        while (getchar() != '\n');
        return;
    }
    
    if (pickup_letter(system, letter_id, office_id)) {
        printf("Письмо успешно доставлено!\n");
    } else {
        printf("Ошибка при доставке письма!\n");
    }
}

void get_letters_ui(PostalSystem *system) {
    char filename[100];
    
    printf("Введите имя файла для экспорта: ");
    if (scanf("%99s", filename) != 1) {
        printf("Неверный ввод!\n");
        while (getchar() != '\n');
        return;
    }
    
    if (get_all_letters(system, filename)) {
        printf("Список писем экспортирован в %s\n", filename);
    } else {
        printf("Ошибка при экспорте!\n");
    }
}

void simulate_delivery_ui(PostalSystem *system) {
    int iterations;
    
    printf("Введите количество итераций симуляции: ");
    if (scanf("%d", &iterations) != 1) {
        printf("Неверный ввод!\n");
        while (getchar() != '\n');
        return;
    }
    
    printf("Запуск симуляции доставки (%d итераций)...\n", iterations);
    for (int i = 0; i < iterations; i++) {
        simulate_mail_delivery(system);
        usleep(200000); // 0.2 секунды
        printf("Итерация %d завершена\n", i + 1);
    }
    printf("Симуляция завершена!\n");
}

void demo_mode(PostalSystem *system) {
    printf("\n=== ДЕМОНСТРАЦИОННЫЙ РЕЖИМ ===\n");
    
    // Создаем тестовые отделения
    unsigned int connections1[] = {2, 3};
    unsigned int connections2[] = {1, 4, 5};
    unsigned int connections3[] = {1, 4};
    unsigned int connections4[] = {2, 3, 6};
    unsigned int connections5[] = {2, 6};
    unsigned int connections6[] = {4, 5};
    
    add_post_office(system, 1, 5, connections1, 2);
    add_post_office(system, 2, 5, connections2, 3);
    add_post_office(system, 3, 5, connections3, 2);
    add_post_office(system, 4, 5, connections4, 3);
    add_post_office(system, 5, 5, connections5, 2);
    add_post_office(system, 6, 5, connections6, 2);
    
    // Добавляем тестовые письма
    add_letter(system, LETTER_REGULAR, 1, 1, 6, "Демо письмо 1");
    add_letter(system, LETTER_URGENT, 5, 2, 5, "Срочное демо письмо");
    add_letter(system, LETTER_REGISTERED, 3, 3, 4, "Заказное демо письмо");
    add_letter(system, LETTER_REGULAR, 2, 1, 3, "Обычное демо письмо");
    
    printf("Демонстрационная система инициализирована!\n");
    print_system_state(system);
}

int main(int argc, char *argv[]) {
    char *mappings_file = "mappings.txt";
    char *log_file = "postal_system.log";
    
    if (argc >= 3) {
        mappings_file = argv[1];
        log_file = argv[2];
    } else {
        printf("Использование: %s <файл_маппингов> <файл_логов>\n", argv[0]);
        printf("Запуск в демонстрационном режиме...\n");
    }
    
    printf("=== ЗАПУСК ПОЧТОВОГО СЕРВИСА ===\n");
    printf("Файл маппингов: %s\n", mappings_file);
    printf("Файл логов: %s\n", log_file);
    
    PostalSystem system = create_postal_system(log_file);
    
    if (argc >= 3) {
        if (!load_mappings(&system, mappings_file)) {
            printf("Ошибка при загрузке маппингов!\n");
            delete_postal_system(&system);
            return 1;
        }
    } else {
        demo_mode(&system);
    }
    
    int running = 1;
    while (running) {
        print_menu();
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("Неверный ввод!\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch (choice) {
            case 1: add_office_ui(&system); break;
            case 2: remove_office_ui(&system); break;
            case 3: add_letter_ui(&system); break;
            case 4: mark_undelivered_ui(&system); break;
            case 5: pickup_letter_ui(&system); break;
            case 6: get_letters_ui(&system); break;
            case 7: simulate_delivery_ui(&system); break;
            case 8: print_system_state(&system); break;
            case 9: 
                running = 0;
                printf("Завершение работы...\n");
                break;
            default:
                printf("Неверный выбор!\n");
        }
    }
    
    delete_postal_system(&system);
    printf("Программа завершена. Логи сохранены в %s\n", log_file);
    return 0;
}
