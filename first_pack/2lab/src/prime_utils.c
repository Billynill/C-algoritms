#include "prime_utils.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

long long estimate_upper_bound(int n) {
    if (n <= 0) return 0;
    if (n < 6) return 15;

    double dn = (double)n;
    double est = dn * (log(dn) + log(log(dn))) + 3.0;
    if (est < dn) est = dn;
    if (est > (double)LLONG_MAX) return 0;

    long long u = (long long)est;
    if (u < 15) u = 15;
    return u;
}

PrimeStatusCode find_nth_prime(int n, long long* result) {
    if (n <= 0 || result == NULL) {
        return PRIME_ERROR_INVALID_INPUT;
    }
    if (n == 1) { *result = 2; return PRIME_SUCCESS; }

    long long U = estimate_upper_bound(n);
    if (U <= 0) return PRIME_ERROR_OVERFLOW;

    // 0 — потенциально простое, 1 — составное
    unsigned char* is_comp = (unsigned char*)calloc((size_t)U + 1, sizeof(unsigned char));
    if (!is_comp) return PRIME_ERROR_MEMORY;

    if (U >= 0) is_comp[0] = 1;
    if (U >= 1) is_comp[1] = 1;

    for (long long p = 2; p * p <= U; ++p) {
        if (!is_comp[p]) {
            long long start = p * p;
            for (long long m = start; m <= U; m += p) {
                is_comp[m] = 1;
            }
        }
    }

    int count = 0;
    long long answer = -1;
    for (long long x = 2; x <= U; ++x) {
        if (!is_comp[x]) {
            ++count;
            if (count == n) {
                answer = x;
                break;
            }
        }
    }

    free(is_comp);

    if (answer < 0) return PRIME_ERROR_INTERNAL;
    *result = answer;
    return PRIME_SUCCESS;
    
}