#include <omp.h>
#include <bits/types/clock_t.h>
#include <bits/types/struct_timeval.h>
#include <stdlib.h>
#include <float.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include "common.h"

uint32_t *generate_rand_numbers(uint32_t count) {
    uint32_t *result = calloc(count, sizeof(uint32_t));
    for (int i = 0; i < count; i++) {
        result[i] = rand();
    }
    return result;
}

int comparator(const void *val1, const void *val2) {
    return (*(int32_t *) val1) - (*(int32_t *) val2);
}

/**
 * @param cycles_number
 * @param elements_number
 * @param sort sorting function
 * @param sort_name
 */
void test_sort(int64_t cycles_number, int64_t elements_number, void (*sort)(uint32_t *, int64_t), char *sort_name) {
#pragma omp single
{
    printf("Generating %ld number for %s\n", elements_number, sort_name);

    uint32_t *src_arr = generate_rand_numbers(elements_number);

    uint32_t *arr_to_sort = calloc(elements_number, sizeof(uint32_t));

    double min_time = DBL_MAX;
    double max_time = DBL_MIN;

    long double total_time = 0;
    printf("Starting %ld sorts of %ld elements. Sort type: %s\n", cycles_number, elements_number, sort_name);
    struct timeval time1, time2;
    clock_t start_t;
    clock_t end_t;
    double time;
    for (int i = 0; i < cycles_number; ++i) {
        bool err = false;
        memcpy(arr_to_sort, src_arr, sizeof(uint32_t) * elements_number);
        start_t = clock();
        gettimeofday(&time1, NULL);

        sort(arr_to_sort, elements_number);

        end_t = clock();
        gettimeofday(&time2, NULL);

        time = (double) (time2.tv_usec - time1.tv_usec) / 1000000 + (double) (time2.tv_sec - time1.tv_sec);
        if (time > max_time) {
            max_time = time;
        }
        if (time < min_time) {
            min_time = time;
        }

        printf("%d sort is done for %.2fs\n", i + 1, time);

        total_time += time;
    }

    free(src_arr);
    free(arr_to_sort);

    puts("___________________________");
    printf("Mean time is: %Lf seconds\n", total_time / cycles_number);
    printf("Time varies between:\n"
           "    min_time: %f seconds\n"
           "    max_time: %f seconds\n", min_time, max_time);
    printf("%ld sorts of %ld elements took %Lfs\n", cycles_number, elements_number, total_time);
}
}