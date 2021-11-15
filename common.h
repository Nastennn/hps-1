#ifndef HPS1_COMMON_H
#define HPS1_COMMON_H
#define ELEM_NUM 50000000
#define CYCLE_NUM 10

#include <stdint-gcc.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void test_sort(int64_t cycles_number, int64_t elements_number, void (*sort)(uint32_t *, int64_t), char *sort_name);

#endif //HPS1_COMMON_H