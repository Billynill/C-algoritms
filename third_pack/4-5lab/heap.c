#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

static void heapify_up(Heap *h, size_t index) {
    if (index == 0) return;
    
    size_t parent = (index - 1) / 2;
    if (h->data[index] < h->data[parent]) {
        int temp = h->data[index];
        h->data[index] = h->data[parent];
        h->data[parent] = temp;
        heapify_up(h, parent);
    }
}

static void heapify_down(Heap *h, size_t index) {
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;
    size_t smallest = index;
    
    if (left < h->size && h->data[left] < h->data[smallest]) {
        smallest = left;
    }
    
    if (right < h->size && h->data[right] < h->data[smallest]) {
        smallest = right;
    }
    
    if (smallest != index) {
        int temp = h->data[index];
        h->data[index] = h->data[smallest];
        h->data[smallest] = temp;
        heapify_down(h, smallest);
    }
}

Heap create_heap(size_t initial_capacity) {
    Heap h = {NULL, 0, 0};
    
    if (initial_capacity > 0) {
        h.data = malloc(initial_capacity * sizeof(int));
        if (h.data != NULL) {
            h.capacity = initial_capacity;
        }
    }
    
    return h;
}

void delete_heap(Heap *h) {
    if (h == NULL) return;
    
    free(h->data);
    h->data = NULL;
    h->size = 0;
    h->capacity = 0;
}

int is_empty_heap(const Heap *h) {
    return h == NULL || h->size == 0;
}

size_t size_heap(const Heap *h) {
    return h != NULL ? h->size : 0;
}

int peek_heap(const Heap *h) {
    if (is_empty_heap(h)) {
        return 0; // или другое значение по умолчанию
    }
    return h->data[0];
}

void push_heap(Heap *h, int value) {
    if (h == NULL) return;
    
    if (h->size >= h->capacity) {
        size_t new_capacity = h->capacity == 0 ? 1 : h->capacity * 2;
        int *new_data = realloc(h->data, new_capacity * sizeof(int));
        if (new_data == NULL) return;
        
        h->data = new_data;
        h->capacity = new_capacity;
    }
    
    h->data[h->size] = value;
    heapify_up(h, h->size);
    h->size++;
}

int pop_heap(Heap *h) {
    if (is_empty_heap(h)) {
        return 0;
    }
    
    int result = h->data[0];
    h->size--;
    
    if (h->size > 0) {
        h->data[0] = h->data[h->size];
        heapify_down(h, 0);
    }
    
    return result;
}

Heap build_heap(const int *array, size_t n) {
    Heap h = create_heap(n);
    
    if (h.data != NULL && array != NULL) {
        for (size_t i = 0; i < n; i++) {
            h.data[i] = array[i];
        }
        h.size = n;
        
        // Построение кучи
        for (int i = (int)n / 2 - 1; i >= 0; i--) {
            heapify_down(&h, (size_t)i);
        }
    }
    
    return h;
}

int is_equal_heap(const Heap *h1, const Heap *h2) {
    if (h1 == NULL || h2 == NULL) return 0;
    if (h1->size != h2->size) return 0;
    
    for (size_t i = 0; i < h1->size; i++) {
        if (h1->data[i] != h2->data[i]) {
            return 0;
        }
    }
    
    return 1;
}
