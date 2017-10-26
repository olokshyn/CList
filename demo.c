#include <stdio.h>

#include "list.h"

#ifndef LIST_DEFINED_INT
DEFINE_LIST(int)
DEFINE_LIST_OPS_FOR_BUILTINS(int)
#define LIST_DEFINED_INT
#endif

#ifndef LIST_DEFINED_DOUBLE
DEFINE_LIST(double)
DEFINE_LIST_OPS_FOR_BUILTINS(double)
#define LIST_DEFINED_DOUBLE
#endif

void test_list_int() {
    LIST_TYPE(int) list = list_create(int);
    if (!list) {
        goto exit;
    }

    list_push_back(int, list, 10);
    list_push_back(int, list, 20);
    list_push_back(int, list, 30);

    LIST_ITER_TYPE(int) iter = list_find_by_value(int, list, 20);
    if (!list_iter_valid(iter)) {
        goto destroy_list;
    }
    list_insert(int, iter, 15);

    list_for_each(int, list, var) {
        printf("%d\n", list_var_value(var));
    }
    printf("\n");

    LIST_TYPE(int) another_list = list_create(int);
    if (!another_list) {
        goto destroy_list;
    }

    list_copy(int, list, another_list);
    list_move(int, list, another_list);

    iter = list_find_by_value(int, another_list, 15);
    if (!list_iter_valid(iter)) {
        goto destroy_another_list;
    }

    list_remove(int, iter);
    list_remove_by_value(int, another_list, 20);

    for (LIST_ITER_TYPE(int) i = list_begin(int, another_list);
         list_iter_valid(i);
         list_next(int, i)) {
        printf("%d\n", list_iter_value(i));
    }
    printf("\n");

    list_clear(int, another_list);

destroy_another_list:
    list_destroy(int, another_list);
destroy_list:
    list_destroy(int, list);
exit:
    return;
}

void test_int_double() {
    LIST_TYPE(double) list = list_create(double);
    if (!list) {
        goto exit;
    }

    list_push_back(double, list, 0.3);
    list_push_back(double, list, 0.5);
    list_push_back(double, list, 1.8);

    LIST_ITER_TYPE(double) iter = list_begin(double, list);
    list_next(double, iter);
    list_next(double, iter);

    for (; list_iter_valid(iter); list_prev(double, iter)) {
        printf("%f\n", list_iter_value(iter));
    }
    printf("\n");

    list_clear(double, list);

    list_push_back(double, list, 0.3);
    list_push_back(double, list, 0.5);
    list_push_back(double, list, 1.8);

    list_for_each_reversed(double, list, var) {
        printf("%f\n", list_var_value(var));
    }
    printf("\n");

    printf("length = %zu\n", list_len(list));

    list_destroy(double, list);
exit:
    return;
}

int main() {
    test_list_int();
    test_int_double();
    return 0;
}
