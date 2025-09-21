//
// Created by Рустам on 9/17/25.
//

#include <stdio.h>
#include <stdlib.h>
#include "prime_utils.h"

static int read_int(int *out) {
    if (scanf("%d", out) != 1) return 0;
    return 1;
}

int main() {
    int T;
    if (!read_int(&T) || T <= 0) {
        fprintf(stderr, "Ошибка ввода: T должно быть положительным целым числом.\n");
        return 1;
    }

    int *queries = (int*)malloc((size_t)T * sizeof(int));
    if (!queries) {
        fprintf(stderr, "Ошибка: не удалось выделить память под массив запросов.\n");
        return 1;
    }

    for (int i = 0; i < T; ++i) {
        if (!read_int(&queries[i])) {
            fprintf(stderr,"Ошибка ввода: запрос #%d не является целым числом.\n", i + 1);
            free(queries);
            return 1;
        }
        if (queries[i] <= 0) {
            fprintf(stderr, "Ошибка ввода: n в запросе #%d должно быть > 0.\n", i + 1);
            free(queries);
            return 1;
        }
    }

    for (int i = 0; i < T; ++i) {
        long long prime = 0;
        PrimeStatusCode st = find_nth_prime(queries[i], &prime);
        switch (st) {
            case PRIME_SUCCESS:
                printf("%lld\n", prime);
                break;
            case PRIME_ERROR_INVALID_INPUT:
                fprintf(stderr,"Ошибка: некорректный запрос n=%d (должно быть n>=1).\n", queries[i]);
                break;
            case PRIME_ERROR_MEMORY:
                fprintf(stderr, "Ошибка: нехватка памяти при вычислении для n=%d.\n", queries[i]);
                break;
            case PRIME_ERROR_OVERFLOW:
                fprintf(stderr, "Ошибка: слишком большое n=%d — верхняя граница не помещается в тип long long.\n", queries[i]);
                break;
            default:
                fprintf(stderr, "Внутренняя ошибка при обработке n=%d.\n", queries[i]);
                break;
        }
    }
    free(queries);
    return 0;
}