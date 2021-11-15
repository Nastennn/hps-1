#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>
#include "common.h"

uint32_t temp[ELEM_NUM];

int min(int a, int b) {
    if (a < b) {
        return a;
    } else
        return b;
}

int insertion_sort(uint32_t *arr, int low, int high) {
    int t, d;
    for (int c = 1 + low; c <= high; c++) {
        d = c;
        while (d > low && arr[d - 1] > arr[d]) {
            t = arr[d];
            arr[d] = arr[d - 1];
            arr[d - 1] = t;
            d--;
        }
    }
    return 0;
}


void merge(uint32_t *arr, int beg, int mid, int end) {
    int i = beg, j = mid + 1, index = beg, k;
    while ((i <= mid) && (j <= end)) {
        if (arr[i] < arr[j]) {
            temp[index] = arr[i];
            i++;
        } else {
            temp[index] = arr[j];
            j++;
        }
        index++;
    }
    if (i > mid) {
        while (j <= end) {
            temp[index] = arr[j];
            j++;
            index++;
        }
    } else {
        while (i <= mid) {
            temp[index] = arr[i];
            i++;
            index++;
        }
    }
    for (k = beg; k <= end; k++) {
        arr[k] = temp[k];
    }
}

void timsort(uint32_t *arr, int64_t size) {
    for (int i = 0; i < size; i = i + CYCLE_NUM) {
        insertion_sort(arr, i, min((i + 31), (size - 1)));
    }
    for (int j = CYCLE_NUM; j < size; j = 2 * j) {
        for (int beg = 0; beg < size; beg = beg + 2 * j) {
            int mid = min((beg + j - 1), (size - 1));
            int end = min((beg + 2 * j - 1), (size - 1));
            merge(arr, beg, mid, end);
        }
    }
}


void parallel_timsort(uint32_t *arr, int64_t size) {
#pragma omp parallel
    {
#pragma omp for
        for (int i = 0; i < size; i = i + CYCLE_NUM) {
            insertion_sort(arr, i, min((i + 31), (size - 1)));
        }
        for (int j = CYCLE_NUM; j < size; j = 2 * j) {
            for (int beg = 0; beg < size; beg = beg + 2 * j) {
                int mid = min((beg + j - 1), (size - 1));
                int end = min((beg + 2 * j - 1), (size - 1));
                merge(arr, beg, mid, end);
            }
        }
    }
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
    test_sort(cycles, elements, timsort, "timsort");
    test_sort(cycles, elements, parallel_timsort, "parallel_timsort");
    return 0;

}