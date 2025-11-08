#include "text_formatter.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Проверка, является ли символ частью слова
bool is_word_character(char c) {
    return isalnum(c) || c == '_' || c == '-' || c == '\''; // Читаемые символы для слов
}

// Найти конец слова (позиция после последнего символа слова)
int find_word_end(const char *text, int start) {
    int pos = start;
    while (text[pos] && is_word_character(text[pos])) {
        pos++;
    }
    return pos;
}

// Найти начало следующего слова
int find_word_start(const char *text, int start) {
    int pos = start;
    while (text[pos] && !is_word_character(text[pos])) {
        pos++;
    }
    return pos;
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
        
        // Находим, сколько символов можем взять в текущую строку
        int line_end = current_pos + MAX_LINE_LENGTH;
        if (line_end > len) {
            line_end = len;
        } else {
            // Ищем место для разрыва по границе слова
            if (line_end < len && is_word_character(line[line_end])) {
                // Находим начало текущего слова
                int word_start = line_end;
                while (word_start > current_pos && is_word_character(line[word_start - 1])) {
                    word_start--;
                }
                
                // Если слово начинается не с начала строки, переносим на следующую строку
                if (word_start > current_pos) {
                    line_end = word_start;
                }
            }
        }
        
        // Записываем строку в выходной файл
        if (current_pos < line_end) {
            // Убеждаемся, что первый символ - читаемый
            while (current_pos < line_end && isspace(line[current_pos])) {
                current_pos++;
            }
            
            if (current_pos < line_end) {
                // Копируем подстроку
                char output_line[MAX_LINE_LENGTH + 2] = {0}; // +2 для \n и \0
                strncpy(output_line, line + current_pos, line_end - current_pos);
                
                // Добавляем равномерно пробелы между словами
                if (line_end - current_pos < MAX_LINE_LENGTH && line_end < len) {
                    // Считаем пробелы и слова
                    int space_count = 0;
                    int word_count = 0;
                    int temp_pos = current_pos;
                    
                    while (temp_pos < line_end) {
                        if (isspace(line[temp_pos])) {
                            space_count++;
                            temp_pos++;
                        } else {
                            word_count++;
                            temp_pos = find_word_end(line, temp_pos);
                        }
                    }
                    
                    if (word_count > 1 && space_count > 0) {
                        // Равномерно распределяем пробелы
                        int total_spaces_needed = MAX_LINE_LENGTH - (line_end - current_pos - space_count);
                        int spaces_between_words = total_spaces_needed / (word_count - 1);
                        int extra_spaces = total_spaces_needed % (word_count - 1);
                        
                        // Перестраиваем строку с равномерными пробелами
                        char formatted_line[MAX_LINE_LENGTH + 1] = {0};
                        int formatted_pos = 0;
                        temp_pos = current_pos;
                        int current_word = 0;
                        
                        while (temp_pos < line_end && formatted_pos < MAX_LINE_LENGTH) {
                            if (isspace(line[temp_pos])) {
                                temp_pos++;
                            } else {
                                // Копируем слово
                                int word_start = temp_pos;
                                int word_end = find_word_end(line, word_start);
                                
                                for (int i = word_start; i < word_end && formatted_pos < MAX_LINE_LENGTH; i++) {
                                    formatted_line[formatted_pos++] = line[i];
                                }
                                
                                // Добавляем пробелы после слова (кроме последнего)
                                if (current_word < word_count - 1) {
                                    int spaces_to_add = spaces_between_words;
                                    if (current_word < extra_spaces) {
                                        spaces_to_add++;
                                    }
                                    
                                    for (int i = 0; i < spaces_to_add && formatted_pos < MAX_LINE_LENGTH; i++) {
                                        formatted_line[formatted_pos++] = ' ';
                                    }
                                }
                                
                                temp_pos = word_end;
                                current_word++;
                            }
                        }
                        
                        strcpy(output_line, formatted_line);
                    }
                }
                
                // Убеждаемся, что строка не заканчивается пробелом
                int output_len = strlen(output_line);
                while (output_len > 0 && isspace(output_line[output_len - 1])) {
                    output_line[output_len - 1] = '\0';
                    output_len--;
                }
                
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
    
    char line[10000]; // Буфер для чтения строк
    int total_lines = 0;
    
    printf("Форматирование файла...\n");
    
    while (fgets(line, sizeof(line), input)) {
        // Убираем символ новой строки
        line[strcspn(line, "\n")] = '\0';
        
        int lines_written = split_line(line, output);
        total_lines += lines_written;
        
        printf("Обработана строка, создано %d строк(и)\n", lines_written);
    }
    
    fclose(input);
    fclose(output);
    
    printf("Форматирование завершено. Создано %d строк в файле '%s'\n", total_lines, output_path);
    return true;
}