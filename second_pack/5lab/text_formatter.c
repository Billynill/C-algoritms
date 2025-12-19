#include "text_formatter.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Проверка, является ли символ частью слова
bool is_word_character(char c) {
    return isalnum(c) || c == '_' || c == '-' || c == '\'';
}

// Найти конец слова
int find_word_end(const char *text, int start) {
    int pos = start;
    while (text[pos] && is_word_character(text[pos])) {
        pos++;
    }
    return pos;
}

// НОВАЯ ФУНКЦИЯ: Растянуть строку пробелами до заданной длины
static void justify_line(char *line, int target_length) {
    int current_len = strlen(line);
    if (current_len >= target_length) return;
    
    // Считаем пробелы и слова
    int space_positions[100]; // позиции пробелов
    int space_count = 0;
    int word_count = 0;
    
    // Находим все пробелы
    for (int i = 0; i < current_len; i++) {
        if (isspace(line[i])) {
            space_positions[space_count++] = i;
        } else if (i == 0 || isspace(line[i-1])) {
            word_count++;
        }
    }
    
    if (word_count <= 1 || space_count == 0) {
        // Нельзя распределить пробелы - просто добавляем в конец
        while (current_len < target_length) {
            line[current_len++] = ' ';
        }
        line[current_len] = '\0';
        return;
    }
    
    // Сколько пробелов нужно добавить
    int spaces_to_add = target_length - current_len;
    int spaces_per_gap = spaces_to_add / space_count;
    int extra_spaces = spaces_to_add % space_count;
    
    // Создаем новую строку
    char new_line[target_length + 1];
    int new_pos = 0;
    int old_pos = 0;
    
    for (int i = 0; i < space_count; i++) {
        // Копируем текст до пробела
        int chars_to_copy = space_positions[i] - old_pos;
        strncpy(new_line + new_pos, line + old_pos, chars_to_copy);
        new_pos += chars_to_copy;
        old_pos = space_positions[i];
        
        // Добавляем оригинальный пробел + дополнительные
        new_line[new_pos++] = ' '; // оригинальный пробел
        for (int j = 0; j < spaces_per_gap; j++) {
            new_line[new_pos++] = ' ';
        }
        if (i < extra_spaces) {
            new_line[new_pos++] = ' ';
        }
        
        old_pos++; // пропускаем оригинальный пробел
    }
    
    // Копируем остаток строки
    int remaining = current_len - old_pos;
    if (remaining > 0) {
        strncpy(new_line + new_pos, line + old_pos, remaining);
        new_pos += remaining;
    }
    
    new_line[new_pos] = '\0';
    strcpy(line, new_line);
}

// Разбить строку на строки по 80 символов с переносом слов
int split_line(char *line, FILE *output) {
    int line_count = 0;
    int len = strlen(line);
    int current_pos = 0;
    
    while (current_pos < len) {
        // Пропускаем начальные пробелы
        while (current_pos < len && isspace(line[current_pos])) {
            current_pos++;
        }
        
        if (current_pos >= len) break;
        
        // Определяем конец текущей строки (максимум 80 символов)
        int line_end = current_pos + MAX_LINE_LENGTH;
        if (line_end > len) {
            line_end = len;
        } else {
            // Проверяем, не разрезаем ли мы слово
            if (line_end < len && is_word_character(line[line_end])) {
                // Ищем начало слова, которое разрезали
                int word_start = line_end;
                while (word_start > current_pos && is_word_character(line[word_start - 1])) {
                    word_start--;
                }
                
                // Если слово начинается не в начале строки, переносим его целиком
                if (word_start > current_pos) {
                    line_end = word_start;
                }
            }
        }
        
        // Извлекаем подстроку
        if (current_pos < line_end) {
            char output_line[MAX_LINE_LENGTH + 1] = {0};
            strncpy(output_line, line + current_pos, line_end - current_pos);
            output_line[line_end - current_pos] = '\0';
            
            // Удаляем начальные пробелы (первый символ должен быть читаемым)
            int start = 0;
            while (output_line[start] && isspace(output_line[start])) {
                start++;
            }
            
            if (output_line[start]) {
                // Сдвигаем строку, если были начальные пробелы
                if (start > 0) {
                    memmove(output_line, output_line + start, strlen(output_line + start) + 1);
                }
                
                // Удаляем конечные пробелы
                int end = strlen(output_line) - 1;
                while (end >= 0 && isspace(output_line[end])) {
                    output_line[end--] = '\0';
                }
                
                // ВАЖНОЕ ИСПРАВЛЕНИЕ: Растягиваем пробелами ТОЛЬКО если нужно
                int current_length = strlen(output_line);
                if (current_length < MAX_LINE_LENGTH && line_end < len) {
                    // Это не последняя строка - растягиваем до 80 символов
                    justify_line(output_line, MAX_LINE_LENGTH);
                }
                // Иначе (последняя строка или строка уже 80 символов) - оставляем как есть
                
                fprintf(output, "%s\n", output_line);
                line_count++;
            }
            
            current_pos = line_end;
        }
    }
    
    return line_count;
}

// Основная функция форматирования файла
bool format_file(const char *input_path, const char *output_path) {
    FILE *input = fopen(input_path, "r");
    if (!input) {
        printf("Ошибка: не удалось открыть входной файл '%s'\n", input_path);
        return false;
    }
    
    FILE *output = fopen(output_path, "w");
    if (!output) {
        printf("Ошибка: не удалось создать выходной файл '%s'\n", output_path);
        fclose(input);
        return false;
    }
    
    char line[10000];
    int total_lines = 0;
    
    printf("Форматирование файла...\n");
    
    while (fgets(line, sizeof(line), input)) {
        line[strcspn(line, "\n")] = '\0';
        
        int lines_written = split_line(line, output);
        total_lines += lines_written;
    }
    
    fclose(input);
    fclose(output);
    
    printf("Форматирование завершено. Создано %d строк в файле '%s'\n", total_lines, output_path);
    return true;
}