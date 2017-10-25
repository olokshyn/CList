#include <stdio.h>

#include "list.h"

#ifndef LIST_DEFINED_INT
DEFINE_LIST(int)
#define LIST_DEFINED_INT
#endif

#ifndef LIST_DEFINED_DOUBLE
DEFINE_LIST(double)
#define LIST_DEFINED_DOUBLE
#endif

void test_list_int() {
    LIST_TYPE(int) list = create_list(int);
    if (!list) {
        goto exit;
    }

    push_back(int, list, 10);
    push_back(int, list, 20);
    push_back(int, list, 30);

    LIST_ITER_TYPE(int) iter = find_by_value(int, list, 20);
    if (!iter_valid(iter)) {
        goto destroy_list;
    }
    insert(int, iter, 15);

    for_each_in_list(int, list, var) {
        printf("%d\n", var_value(var));
    }
    printf("\n");

    LIST_TYPE(int) another_list = create_list(int);
    if (!another_list) {
        goto destroy_list;
    }

    copy_list(int, list, another_list);
    move_list(int, list, another_list);

    iter = find_by_value(int, another_list, 15);
    if (!iter_valid(iter)) {
        goto destroy_another_list;
    }

    remove(int, iter);
    remove_by_value(int, another_list, 20);

    for (LIST_ITER_TYPE(int) i = begin(int, another_list);
            iter_valid(i);
            next(int, i)) {
        printf("%d\n", iter_value(i));
    }
    printf("\n");

    clear_list(int, another_list);

destroy_another_list:
    destroy_list(int, another_list);
destroy_list:
    destroy_list(int, list);
exit:
    return;
}

void test_int_double() {
    LIST_TYPE(double) list = create_list(double);
    if (!list) {
        goto exit;
    }

    push_back(double, list, 0.3);
    push_back(double, list, 0.5);
    push_back(double, list, 1.8);

    LIST_ITER_TYPE(double) iter = begin(double, list);
    next(double, iter);
    next(double, iter);

    for (; iter_valid(iter); prev(double, iter)) {
        printf("%f\n", iter_value(iter));
    }
    printf("\n");

    clear_list(double, list);

    push_back(double, list, 0.3);
    push_back(double, list, 0.5);
    push_back(double, list, 1.8);

    for_each_in_list_reversed(double, list, var) {
        printf("%f\n", var_value(var));
    }
    printf("\n");

    printf("length = %zu\n", list_len(list));

    destroy_list(double, list);
exit:
    return;
}

int main() {
    test_list_int();
    test_int_double();
    return 0;
}
