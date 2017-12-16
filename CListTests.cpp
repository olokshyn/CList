//
// Created by Oleg on 10/25/17.
//

#include "gtest/gtest.h"

extern "C"
{
#include "list.h"
}

#ifndef LIST_DEFINED_SIZE_T
DEFINE_LIST(size_t)
DEFINE_LIST_OPS_FOR_BUILTINS(size_t)
#define LIST_DEFINED_SIZE_T
#endif

#ifndef LIST_DEFINED_DOUBLE
DEFINE_LIST(double)
DEFINE_LIST_OPS_FOR_BUILTINS(double)
#define LIST_DEFINED_DOUBLE
#endif

#ifndef LIST_DEFINED_INT
DEFINE_LIST(int)
DEFINE_LIST_OPS_FOR_BUILTINS(int)
#define LIST_DEFINED_INT
#endif

struct test_type
{
    int field_i;
    double field_d;
};

int test_type_cmp(test_type* a, test_type* b)
{
    if (a->field_i == b->field_i
        && a->field_d == b->field_d)
    {
        return 0;
    }
    return -1;
}

#ifndef LIST_DEFINED_TEST_TYPE
DEFINE_LIST(test_type)
#define LIST_DEFINED_TEST_TYPE
#endif

TEST(CListTest, Creation)
{
    LIST_TYPE(size_t) list = list_create(size_t);
    ASSERT_TRUE(list);

    EXPECT_EQ(0, list_len(list));
    EXPECT_TRUE(list_empty(list));
    size_t count = 0;
    list_for_each(size_t, list, var) {
        ++count;
    }
    EXPECT_EQ(0, count);
    ASSERT_TRUE(list_destroy(size_t, list));
}

TEST(CListTest, PushBack)
{
    LIST_TYPE(double) list = list_create(double);
    ASSERT_TRUE(list);

    EXPECT_TRUE(list_push_back(double, list, 1.0));
    EXPECT_TRUE(list_push_back(double, list, 2.3));
    EXPECT_EQ(2, list_len(list));
    LIST_ITER_TYPE(double) iter = list_begin(double, list);
    EXPECT_EQ(1.0, list_iter_value(iter));
    list_next(double, iter);
    EXPECT_EQ(2.3, list_iter_value(iter));
    list_next(double, iter);
    EXPECT_FALSE(list_iter_valid(iter));

    EXPECT_TRUE(list_clear(double, list));

    for (double pos = 0.0; pos <= 1.0; pos += 0.2)
    {
        EXPECT_TRUE(list_push_back(double, list, pos));
    }
    EXPECT_EQ(6, list_len(list));
    EXPECT_FALSE(list_empty(list));

    size_t index = 0;
    list_for_each(double, list, var)
    {
        EXPECT_EQ(index++ * 0.2, list_var_value(var));
    }
    EXPECT_EQ(6, index);

    ASSERT_TRUE(list_destroy(double, list));
}

TEST(CListTest, Insert)
{
    LIST_TYPE(int) list = list_create(int);
    ASSERT_TRUE(list);

    LIST_ITER_TYPE(int) iter = list_begin(int, list);
    for (int i = 0; i != 4; ++i)
    {
        EXPECT_TRUE(list_insert(int, iter, i));
    }
    EXPECT_EQ(4, list_len(list));
    EXPECT_FALSE(list_empty(list));
    EXPECT_EQ(0, list_first(list));
    EXPECT_EQ(3, list_last(list));

    int index = 0;
    list_for_each(int, list, var)
    {
        EXPECT_EQ(index++, list_var_value(var));
    }
    EXPECT_EQ(4, index);

    EXPECT_TRUE(list_push_back(int, list, 7));
    iter = list_find_by_value(int, list, 7);
    EXPECT_TRUE(list_iter_valid(iter));
    for (int i = 4; i != 7; ++i)
    {
        EXPECT_TRUE(list_insert(int, iter, i));
    }
    EXPECT_EQ(8, list_len(list));

    index = 0;
    list_for_each(int, list, var)
    {
        EXPECT_EQ(index++, list_var_value(var));
    }
    EXPECT_EQ(8, index);


    ASSERT_TRUE(list_destroy(int, list));
}

TEST(CListTest, ClearList)
{
    LIST_TYPE(test_type) list = list_create(test_type);
    ASSERT_TRUE(list);

    for (int i = 0; i != 5; ++i)
    {
        test_type data = { i, i + 0.5 };
        EXPECT_TRUE(list_push_back(test_type, list, data));
    }
    EXPECT_EQ(5, list_len(list));

    EXPECT_TRUE(list_clear(test_type, list));
    EXPECT_EQ(0, list_len(list));
    EXPECT_TRUE(list_empty(list));

    for (int i = 0; i != 5; ++i)
    {
        test_type data = { i, i + 0.5 };
        EXPECT_TRUE(list_push_back(test_type, list, data));
    }
    EXPECT_EQ(5, list_len(list));

    int index = 0;
    list_for_each(test_type, list, var)
    {
        EXPECT_EQ(index, list_var_value(var).field_i);
        EXPECT_EQ(index + 0.5, list_var_value(var).field_d);
        ++index;
    }
    EXPECT_EQ(5, index);

    EXPECT_TRUE(list_clear(test_type, list));
    EXPECT_EQ(0, list_len(list));
    EXPECT_TRUE(list_empty(list));

    ASSERT_TRUE(list_destroy(test_type, list));
}

TEST(CListTest, CopyList)
{
    LIST_TYPE(test_type) list = list_create(test_type);
    ASSERT_TRUE(list);

    for (int i = 0; i != 5; ++i)
    {
        test_type data = { i, i + 0.5 };
        EXPECT_TRUE(list_push_back(test_type, list, data));
    }
    EXPECT_EQ(5, list_len(list));

    LIST_TYPE(test_type) other_list = list_create(test_type);
    ASSERT_TRUE(list);

    EXPECT_TRUE(list_copy(test_type, list, other_list));
    EXPECT_EQ(5, list_len(list));
    EXPECT_FALSE(list_empty(list));
    EXPECT_EQ(5, list_len(other_list));
    EXPECT_FALSE(list_empty(list));

    // Check the integrity of the original list
    int index = 0;
    list_for_each(test_type, list, var)
    {
        EXPECT_EQ(index, list_var_value(var).field_i);
        EXPECT_EQ(index + 0.5, list_var_value(var).field_d);
        ++index;
    }
    EXPECT_EQ(5, index);

    // Check the copy
    index = 0;
    list_for_each(test_type, other_list, var)
    {
        EXPECT_EQ(index, list_var_value(var).field_i);
        EXPECT_EQ(index + 0.5, list_var_value(var).field_d);
        ++index;
    }
    EXPECT_EQ(5, index);

    // Test the behavior after the list_clear()
    EXPECT_TRUE(list_clear(test_type, other_list));

    EXPECT_TRUE(list_copy(test_type, list, other_list));
    EXPECT_EQ(5, list_len(list));
    EXPECT_EQ(5, list_len(other_list));

    // Check the integrity of the original list
    index = 0;
    list_for_each(test_type, list, var)
    {
        EXPECT_EQ(index, list_var_value(var).field_i);
        EXPECT_EQ(index + 0.5, list_var_value(var).field_d);
        ++index;
    }
    EXPECT_EQ(5, index);

    // Check the copy
    index = 0;
    list_for_each(test_type, other_list, var)
    {
        EXPECT_EQ(index, list_var_value(var).field_i);
        EXPECT_EQ(index + 0.5, list_var_value(var).field_d);
        ++index;
    }
    EXPECT_EQ(5, index);

    // Check that the copy has it`s own memory
    test_type data = { -1, -1.5 };
    EXPECT_TRUE(list_push_back(test_type, list, data));

    EXPECT_EQ(6, list_len(list));
    EXPECT_EQ(5, list_len(other_list));

    LIST_ITER_TYPE(test_type) iter =
        list_find_by_value_cmp(test_type, list, data, test_type_cmp);
    EXPECT_TRUE(list_iter_valid(iter));
    iter = list_find_by_value_cmp(test_type, other_list, data, test_type_cmp);
    EXPECT_FALSE(list_iter_valid(iter));

    EXPECT_TRUE(list_clear(test_type, other_list));
    EXPECT_EQ(6, list_len(list));
    EXPECT_EQ(0, list_len(other_list));

    // Check the integrity of the original list
    index = 0;
    list_for_each(test_type, list, var)
    {
        if (index == 5)
        {
            break;
        }
        EXPECT_EQ(index, list_var_value(var).field_i);
        EXPECT_EQ(index + 0.5, list_var_value(var).field_d);
        ++index;
    }
    EXPECT_EQ(5, index);
    EXPECT_EQ(0, test_type_cmp(&data, &list_iter_value(list_tail(test_type, list))));

    ASSERT_TRUE(list_destroy(test_type, other_list));
    ASSERT_TRUE(list_destroy(test_type, list));
}

TEST(CListTest, MoveList)
{
    LIST_TYPE(test_type) list = list_create(test_type);
    ASSERT_TRUE(list);

    for (int i = 0; i != 5; ++i)
    {
        test_type data = { i, i + 0.5 };
        EXPECT_TRUE(list_push_back(test_type, list, data));
    }
    EXPECT_EQ(5, list_len(list));

    LIST_TYPE(test_type) other_list = list_create(test_type);
    ASSERT_TRUE(list);

    EXPECT_TRUE(list_move(test_type, list, other_list));
    EXPECT_EQ(0, list_len(list));
    EXPECT_TRUE(list_empty(list));
    EXPECT_EQ(5, list_len(other_list));
    EXPECT_FALSE(list_empty(other_list));

    // Check that the original list is empty
    int index = 0;
    list_for_each(test_type, list, var)
    {
        ++index;
    }
    EXPECT_EQ(0, index);

    // Check the copy
    index = 0;
    list_for_each(test_type, other_list, var)
    {
        EXPECT_EQ(index, list_var_value(var).field_i);
        EXPECT_EQ(index + 0.5, list_var_value(var).field_d);
        ++index;
    }
    EXPECT_EQ(5, index);

    // Test moving backwards
    EXPECT_TRUE(list_move(test_type, other_list, list));
    EXPECT_EQ(5, list_len(list));
    EXPECT_FALSE(list_empty(list));
    EXPECT_EQ(0, list_len(other_list));
    EXPECT_TRUE(list_empty(other_list));

    // Check the integrity of the list
    index = 0;
    list_for_each(test_type, list, var)
    {
        EXPECT_EQ(index, list_var_value(var).field_i);
        EXPECT_EQ(index + 0.5, list_var_value(var).field_d);
        ++index;
    }
    EXPECT_EQ(5, index);

    // Check the copy
    index = 0;
    list_for_each(test_type, other_list, var)
    {
        ++index;
    }
    EXPECT_EQ(0, index);

    // Check the integrity of the moved from list
    test_type data = { -1, -1.5 };
    EXPECT_TRUE(list_push_back(test_type, other_list, data));

    EXPECT_EQ(5, list_len(list));
    EXPECT_EQ(1, list_len(other_list));

    LIST_ITER_TYPE(test_type) iter =
        list_find_by_value_cmp(test_type, list, data, test_type_cmp);
    EXPECT_FALSE(list_iter_valid(iter));
    iter = list_find_by_value_cmp(test_type, other_list, data, test_type_cmp);
    EXPECT_TRUE(list_iter_valid(iter));

    ASSERT_TRUE(list_destroy(test_type, other_list));
    ASSERT_TRUE(list_destroy(test_type, list));
}

TEST(CListTest, FindByValue)
{
    LIST_TYPE(int) simple_list = list_create(int);
    ASSERT_TRUE(simple_list);

    LIST_TYPE(test_type) complex_list = list_create(test_type);
    ASSERT_TRUE(complex_list);

    for (int i = 0; i != 5; ++i)
    {
        EXPECT_TRUE(list_push_back(int, simple_list, i));
        test_type data = { i, i + 0.5 };
        EXPECT_TRUE(list_push_back(test_type, complex_list, data));
    }
    EXPECT_EQ(5, list_len(simple_list));
    EXPECT_EQ(5, list_len(complex_list));

    LIST_ITER_TYPE(int) simple_iter = list_find_by_value(int, simple_list, 3);
    ASSERT_TRUE(list_iter_valid(simple_iter));
    EXPECT_EQ(3, list_iter_value(simple_iter));

    simple_iter = list_find_by_value(int, simple_list, 4);
    ASSERT_TRUE(list_iter_valid(simple_iter));
    EXPECT_EQ(4, list_iter_value(simple_iter));

    simple_iter = list_find_by_value(int, simple_list, 10);
    EXPECT_FALSE(list_iter_valid(simple_iter));

    test_type data = { 3, 3.5 };
    LIST_ITER_TYPE(test_type) complex_iter =
        list_find_by_value_cmp(test_type, complex_list, data, test_type_cmp);
    ASSERT_TRUE(list_iter_valid(complex_iter));
    EXPECT_EQ(3, list_iter_value(complex_iter).field_i);

    data = { 4, 4.5 };
    complex_iter =
        list_find_by_value_cmp(test_type, complex_list, data, test_type_cmp);
    ASSERT_TRUE(list_iter_valid(complex_iter));
    EXPECT_EQ(4, list_iter_value(complex_iter).field_i);

    data = { 10, 10.5 };
    complex_iter =
        list_find_by_value_cmp(test_type, complex_list, data, test_type_cmp);
    EXPECT_FALSE(list_iter_valid(complex_iter));

    EXPECT_TRUE(list_clear(int, simple_list));
    EXPECT_TRUE(list_clear(test_type, complex_list));

    simple_iter = list_find_by_value(int, simple_list, 0);
    EXPECT_FALSE(list_iter_valid(simple_iter));

    data = { 0, 0.0 };
    complex_iter =
        list_find_by_value_cmp(test_type, complex_list, data, test_type_cmp);
    EXPECT_FALSE(list_iter_valid(complex_iter));

    ASSERT_TRUE(list_destroy(test_type, complex_list));
    ASSERT_TRUE(list_destroy(int, simple_list));
}

TEST(CListTest, Remove)
{
    LIST_TYPE(int) simple_list = list_create(int);
    ASSERT_TRUE(simple_list);

    LIST_TYPE(test_type) complex_list = list_create(test_type);
    ASSERT_TRUE(complex_list);

    for (int i = 0; i != 5; ++i)
    {
        EXPECT_TRUE(list_push_back(int, simple_list, i));
        test_type data = { i, i + 0.5 };
        EXPECT_TRUE(list_push_back(test_type, complex_list, data));
    }
    EXPECT_EQ(5, list_len(simple_list));
    EXPECT_EQ(5, list_len(complex_list));

    LIST_ITER_TYPE(int) simple_iter = list_find_by_value(int, simple_list, 3);
    ASSERT_TRUE(list_iter_valid(simple_iter));
    EXPECT_TRUE(list_remove(int, simple_iter));
    EXPECT_EQ(4, list_len(simple_list));

    simple_iter = list_find_by_value(int, simple_list, 1);
    ASSERT_TRUE(list_iter_valid(simple_iter));
    EXPECT_TRUE(list_remove(int, simple_iter));
    EXPECT_EQ(3, list_len(simple_list));

    EXPECT_TRUE(list_remove_by_value(int, simple_list, 4));
    EXPECT_EQ(2, list_len(simple_list));

    EXPECT_FALSE(list_remove_by_value(int, simple_list, 10));
    EXPECT_EQ(2, list_len(simple_list));

    simple_iter = list_begin(int, simple_list);
    ASSERT_TRUE(list_iter_valid(simple_iter));
    EXPECT_EQ(0, list_iter_value(simple_iter));
    list_next(int, simple_iter);
    EXPECT_EQ(2, list_iter_value(simple_iter));
    list_next(int, simple_iter);
    EXPECT_FALSE(list_iter_valid(simple_iter));

    test_type data = { 3, 3.5 };
    LIST_ITER_TYPE(test_type) complex_iter =
            list_find_by_value_cmp(test_type, complex_list, data, test_type_cmp);
    ASSERT_TRUE(list_iter_valid(complex_iter));
    EXPECT_TRUE(list_remove(test_type, complex_iter));
    EXPECT_EQ(4, list_len(complex_list));

    data = { 1, 1.5 };
    complex_iter =
            list_find_by_value_cmp(test_type, complex_list, data, test_type_cmp);
    ASSERT_TRUE(list_iter_valid(complex_iter));
    EXPECT_TRUE(list_remove(test_type, complex_iter));
    EXPECT_EQ(3, list_len(complex_list));

    data = { 4, 4.5 };
    EXPECT_TRUE(list_remove_by_value_cmp(
            test_type, complex_list, data, test_type_cmp));
    EXPECT_EQ(2, list_len(complex_list));

    data = { 10, 10.5 };
    EXPECT_FALSE(list_remove_by_value_cmp(
            test_type, complex_list, data, test_type_cmp));
    EXPECT_EQ(2, list_len(complex_list));

    complex_iter = list_begin(test_type, complex_list);
    ASSERT_TRUE(list_iter_valid(complex_iter));
    EXPECT_EQ(0, list_iter_value(complex_iter).field_i);
    list_next(test_type, complex_iter);
    EXPECT_EQ(2, list_iter_value(complex_iter).field_i);
    list_next(test_type, complex_iter);
    EXPECT_FALSE(list_iter_valid(complex_iter));

    ASSERT_TRUE(list_destroy(test_type, complex_list));
    ASSERT_TRUE(list_destroy(int, simple_list));
}

TEST(CListTest, Pop)
{
    LIST_TYPE(int) simple_list = list_create(int);
    ASSERT_TRUE(simple_list);

    LIST_TYPE(test_type) complex_list = list_create(test_type);
    ASSERT_TRUE(complex_list);

    for (int i = 0; i != 5; ++i)
    {
        EXPECT_TRUE(list_push_back(int, simple_list, i));
        test_type data = { i, i + 0.5 };
        EXPECT_TRUE(list_push_back(test_type, complex_list, data));
    }
    EXPECT_EQ(5, list_len(simple_list));
    EXPECT_EQ(5, list_len(complex_list));

    EXPECT_EQ(0, list_pop_front(int, simple_list));
    EXPECT_EQ(4, list_len(simple_list));
    EXPECT_EQ(0, list_pop_front(test_type, complex_list).field_i);
    EXPECT_EQ(4, list_len(complex_list));

    int index = 1;
    list_for_each(int, simple_list, var) {
        EXPECT_EQ(index++, list_var_value(var));
    }
    EXPECT_EQ(5, index);

    index = 1;
    list_for_each(test_type, complex_list, var) {
        EXPECT_EQ(index++, list_var_value(var).field_i);
    }
    EXPECT_EQ(5, index);

    EXPECT_EQ(4, list_pop_back(int, simple_list));
    EXPECT_EQ(3, list_len(simple_list));
    EXPECT_EQ(4, list_pop_back(test_type, complex_list).field_i);
    EXPECT_EQ(3, list_len(complex_list));

    index = 1;
    list_for_each(int, simple_list, var) {
        EXPECT_EQ(index++, list_var_value(var));
    }
    EXPECT_EQ(4, index);

    index = 1;
    list_for_each(test_type, complex_list, var) {
        EXPECT_EQ(index++, list_var_value(var).field_i);
    }
    EXPECT_EQ(4, index);

    EXPECT_TRUE(list_push_back(int, simple_list, 4));
    EXPECT_EQ(4, list_len(simple_list));
    EXPECT_TRUE(list_insert(int, list_begin(int, simple_list), 0));
    EXPECT_EQ(5, list_len(simple_list));

    index = 0;
    list_for_each(int, simple_list, var) {
        EXPECT_EQ(index++, list_var_value(var));
    }
    EXPECT_EQ(5, index);

    EXPECT_TRUE(list_clear(int, simple_list));

    EXPECT_TRUE(list_push_back(int, simple_list, 5));
    EXPECT_EQ(5, list_pop_front(int, simple_list));
    EXPECT_EQ(0, list_len(simple_list));
    EXPECT_TRUE(list_empty(simple_list));
    EXPECT_TRUE(list_iter_eq(list_head(int, simple_list),
                             list_end(int, simple_list)));
    EXPECT_TRUE(list_iter_eq(list_tail(int, simple_list),
                             list_end(int, simple_list)));

    EXPECT_TRUE(list_push_back(int, simple_list, 5));
    EXPECT_EQ(5, list_pop_back(int, simple_list));
    EXPECT_EQ(0, list_len(simple_list));
    EXPECT_TRUE(list_empty(simple_list));
    EXPECT_TRUE(list_iter_eq(list_head(int, simple_list),
                             list_end(int, simple_list)));
    EXPECT_TRUE(list_iter_eq(list_tail(int, simple_list),
                             list_end(int, simple_list)));

    ASSERT_TRUE(list_destroy(test_type, complex_list));
    ASSERT_TRUE(list_destroy(int, simple_list));
}

TEST(CListTest, Iterator)
{
    LIST_TYPE(test_type) list = list_create(test_type);
    ASSERT_TRUE(list);

    int index = 0;
    list_for_each(test_type, list, var)
    {
        ++index;
    }
    EXPECT_EQ(0, index);

    LIST_ITER_TYPE(test_type) iter = list_begin(test_type, list);
    EXPECT_FALSE(list_iter_valid(iter));

    for (int i = 0; i != 5; ++i)
    {
        test_type data = { i, i + 0.5 };
        list_push_back(test_type, list, data);
    }
    EXPECT_EQ(5, list_len(list));

    index = 0;
    list_for_each(test_type, list, var)
    {
        EXPECT_EQ(index, list_var_value(var).field_i);
        EXPECT_EQ(index + 0.5, list_var_value(var).field_d);
        ++index;
    }
    EXPECT_EQ(5, index);

    index = 4;
    list_for_each_reversed(test_type, list, var)
    {
        EXPECT_EQ(index, list_var_value(var).field_i);
        EXPECT_EQ(index + 0.5, list_var_value(var).field_d);
        --index;
    }
    EXPECT_EQ(-1, index);

    iter = list_head(test_type, list);
    EXPECT_EQ(0, list_iter_value(iter).field_i);
    list_next(test_type, iter);
    EXPECT_EQ(1, list_iter_value(iter).field_i);
    list_next(test_type, iter);
    EXPECT_EQ(2, list_iter_value(iter).field_i);
    list_next(test_type, iter);
    EXPECT_EQ(3, list_iter_value(iter).field_i);
    list_next(test_type, iter);
    EXPECT_EQ(4, list_iter_value(iter).field_i);
    EXPECT_TRUE(list_iter_eq(iter, list_tail(test_type, list)));
    list_next(test_type, iter);
    EXPECT_FALSE(list_iter_valid(iter));

    iter = list_tail(test_type, list);
    EXPECT_EQ(4, list_iter_value(iter).field_i);
    list_prev(test_type, iter);
    EXPECT_EQ(3, list_iter_value(iter).field_i);
    list_prev(test_type, iter);
    EXPECT_EQ(2, list_iter_value(iter).field_i);
    list_prev(test_type, iter);
    EXPECT_EQ(1, list_iter_value(iter).field_i);
    list_prev(test_type, iter);
    EXPECT_EQ(0, list_iter_value(iter).field_i);
    EXPECT_TRUE(list_iter_eq(iter, list_head(test_type, list)));
    list_prev(test_type, iter);
    EXPECT_FALSE(list_iter_valid(iter));

    EXPECT_FALSE(list_iter_valid(list_end(test_type, list)));

    ASSERT_TRUE(list_destroy(test_type, list));
}
