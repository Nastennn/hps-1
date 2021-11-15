#include <stdint-gcc.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "common.h"

void quicksort(uint32_t *arr, int64_t size) {
    int64_t i = 0;
    int64_t j = size - 1;
    uint32_t mid = arr[size / 2];
    do {
        while (arr[i] < mid) {
            i++;
        }
        while (arr[j] > mid) {
            j--;
        }
        if (i <= j) {
            uint32_t tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    } while (i <= j);

    if (j > 0) quicksort(arr, j + 1);
    if (i < size) quicksort(arr + i, size - i);
}

void parallel_quicksort(uint32_t *arr, int64_t size) {
    int64_t i = 0;
    int64_t j = size - 1;
    uint32_t mid = arr[size / 2];
    do {
        while (arr[i] < mid) {
            i++;
        }
        while (arr[j] > mid) {
            j--;
        }
        if (i <= j) {
            uint32_t tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    } while (i <= j);

#pragma omp task shared(arr) if(size > 50000)
    if (j > 0) parallel_quicksort(arr, j + 1);
#pragma omp task shared(arr) if(size > 50000)
    if (i < size) parallel_quicksort(arr + i, size - i);
#pragma omp taskwait
}

int main(int argc, char **argv) {
    int64_t elements = ELEM_NUM;
    int64_t cycles = CYCLE_NUM;
    if (argc > 1) {
        elements = strtol(argv[1], NULL, 10);
    }
    if (argc > 2) {
        cycles = strtol(argv[2], NULL, 10);
    }
    test_sort(cycles, elements, quicksort, "quicksort");
    #pragma omp parallel
    {
        test_sort(cycles, elements, parallel_quicksort, "parallel_quicksort");
    }
    return 0;
}
