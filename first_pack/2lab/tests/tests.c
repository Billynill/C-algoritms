//
// Created by Рустам on 9/17/25.
//

// tests/tests.c
// Простые модульные тесты для prime_utils
// Сборка: вместе с библиотекой prime_utils (см. CMakeLists.txt)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "prime_utils.h"

static int g_failed = 0;
static int g_passed = 0;

#define EXPECT_TRUE(msg, cond) do { \
    if (!(cond)) { \
        fprintf(stderr, "[FAIL] %s (%s:%d)\n", (msg), __FILE__, __LINE__); \
        g_failed++; \
    } else { \
        g_passed++; \
    } \
} while(0)

#define EXPECT_EQ_LL(msg, got, expected) do { \
    long long _g = (got); \
    long long _e = (expected); \
    if (_g != _e) { \
        fprintf(stderr, "[FAIL] %s: got=%lld expected=%lld (%s:%d)\n", \
                (msg), _g, _e, __FILE__, __LINE__); \
        g_failed++; \
    } else { \
        g_passed++; \
    } \
} while(0)

#define EXPECT_STATUS(msg, status, expected) do { \
    PrimeStatusCode _s = (status); \
    PrimeStatusCode _e = (expected); \
    if (_s != _e) { \
        fprintf(stderr, "[FAIL] %s: status=%d expected=%d (%s:%d)\n", \
                (msg), (int)_s, (int)_e, __FILE__, __LINE__); \
        g_failed++; \
    } else { \
        g_passed++; \
    } \
} while(0)

static void test_bounds_basic(void) {
    long long u1 = estimate_upper_bound(1);
    long long u2 = estimate_upper_bound(2);
    long long u5 = estimate_upper_bound(5);
    long long u6 = estimate_upper_bound(6);
    long long u10 = estimate_upper_bound(10);

    EXPECT_EQ_LL("bound(n<=0) -> 0", estimate_upper_bound(0), 0);
    EXPECT_TRUE("bound(n=1) >= 2", u1 >= 2);
    EXPECT_TRUE("bound(n=2) >= 3", u2 >= 3);
    EXPECT_TRUE("bound(n=5) >= 11", u5 >= 11);
    EXPECT_TRUE("bound(n=6) >= 13", u6 >= 13);
    EXPECT_TRUE("bound monotonic-ish: u10 >= u6", u10 >= u6);
}

static void test_find_errors(void) {
    long long out = -1;

    PrimeStatusCode st1 = find_nth_prime(0, &out);
    EXPECT_STATUS("n=0 -> INVALID_INPUT", st1, PRIME_ERROR_INVALID_INPUT);

    PrimeStatusCode st2 = find_nth_prime(-5, &out);
    EXPECT_STATUS("n<0 -> INVALID_INPUT", st2, PRIME_ERROR_INVALID_INPUT);

    PrimeStatusCode st3 = find_nth_prime(10, NULL);
    EXPECT_STATUS("result=NULL -> INVALID_INPUT", st3, PRIME_ERROR_INVALID_INPUT);
}

// --- Корректные значения для первых простых ---
static void test_small_primes(void) {
    struct { int n; long long p; } cases[] = {
        {1, 2},
        {2, 3},
        {3, 5},
        {4, 7},
        {5, 11},
        {6, 13},
        {10, 29},
        {25, 97},
    };
    for (size_t i = 0; i < sizeof(cases)/sizeof(cases[0]); ++i) {
        long long out = 0;
        PrimeStatusCode st = find_nth_prime(cases[i].n, &out);
        char msg[128];
        snprintf(msg, sizeof(msg), "find_nth_prime(%d) status OK", cases[i].n);
        EXPECT_STATUS(msg, st, PRIME_SUCCESS);

        snprintf(msg, sizeof(msg), "find_nth_prime(%d) value", cases[i].n);
        EXPECT_EQ_LL(msg, out, cases[i].p);
    }
}

static void test_known_milestones(void) {
    {
        long long out = 0;
        PrimeStatusCode st = find_nth_prime(100, &out);
        EXPECT_STATUS("n=100 status", st, PRIME_SUCCESS);
        EXPECT_EQ_LL("n=100 value", out, 541);
    }

    {
        long long out = 0;
        PrimeStatusCode st = find_nth_prime(1000, &out);
        EXPECT_STATUS("n=1000 status", st, PRIME_SUCCESS);
        EXPECT_EQ_LL("n=1000 value", out, 7919);
    }

    {
        long long out = 0;
        PrimeStatusCode st = find_nth_prime(10000, &out);
        EXPECT_STATUS("n=10000 status", st, PRIME_SUCCESS);
        EXPECT_EQ_LL("n=10000 value", out, 104729);
    }
}


static void test_stress_large_n(void) {
    const int n = 20000;
    long long out = 0;
    PrimeStatusCode st = find_nth_prime(n, &out);
    EXPECT_STATUS("stress n=20000 status", st, PRIME_SUCCESS);
    EXPECT_TRUE("stress n=20000 out>0", out > 0);
}

int main(void) {
    puts("=== nth-prime tests ===");

    test_bounds_basic();
    test_find_errors();
    test_small_primes();
    test_known_milestones();
    test_stress_large_n();

    printf("Passed: %d, Failed: %d\n", g_passed, g_failed);
    return g_failed == 0 ? 0 : 1;
}