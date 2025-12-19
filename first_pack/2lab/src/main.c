#include <stdio.h>
#include <stdlib.h>
#include "prime_utils.h"

int main(void) {
    
    // ВВОД: количество запросов
    printf("Введите количество запросов T: ");
    int T;
    if (scanf("%d", &T) != 1 || T <= 0) {
        printf("ОШИБКА: T должно быть положительным целым числом!\n");
        return 1;
    }
    
    // ВВОД: номера простых чисел
    printf("\nВведите %d номеров простых чисел (по одному в строке):\n", T);
    
    int* queries = (int*)malloc(T * sizeof(int));
    if (!queries) {
        printf("ОШИБКА: Не удалось выделить память!\n");
        return 1;
    }
    
    for (int i = 0; i < T; i++) {
        printf("n%d = ", i + 1);
        if (scanf("%d", &queries[i]) != 1 || queries[i] <= 0) {
            printf("ОШИБКА: Номер должен быть положительным целым числом!\n");
            free(queries);
            return 1;
        }
    }
    
    // ВЫВОД: результаты
    printf("\n=== РЕЗУЛЬТАТЫ ===\n");
    
    for (int i = 0; i < T; i++) {
        long long prime;
        PrimeStatusCode status = find_nth_prime(queries[i], &prime);
        
        if (status == PRIME_SUCCESS) {
            printf("%d-е простое число = %lld\n", queries[i], prime);
        } else {
            printf("ОШИБКА для n=%d: ", queries[i]);
            
            switch (status) {
                case PRIME_ERROR_INVALID_INPUT:
                    printf("некорректный номер\n");
                    break;
                case PRIME_ERROR_MEMORY:
                    printf("не хватает памяти\n");
                    break;
                case PRIME_ERROR_OVERFLOW:
                    printf("слишком большой номер\n");
                    break;
                case PRIME_ERROR_INTERNAL:
                    printf("внутренняя ошибка алгоритма\n");
                    break;
                default:
                    printf("неизвестная ошибка\n");
            }
        }
    }
    
    printf("\n=== ВСЕГО ОБРАБОТАНО ЗАПРОСОВ: %d ===\n", T);
    
    free(queries);
    return 0;
}
