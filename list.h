//
// Created by oleg on 10/25/17.
//

#ifndef CLIST_LIST_H
#define CLIST_LIST_H

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define DEFINE_LIST(TYPE) \
\
        struct _list__node_##TYPE { \
            TYPE value; \
            struct _list__node_##TYPE* next; \
            struct _list__node_##TYPE* prev; \
        }; \
\
        struct _list__list_##TYPE { \
            struct _list__node_##TYPE* head; \
            struct _list__node_##TYPE* tail; \
            size_t length; \
        }; \
\
        struct _list__iterator_##TYPE { \
            struct _list__node_##TYPE* current; \
            struct _list__list_##TYPE* list; \
        }; \
\
        static struct _list__list_##TYPE* \
        _list__create_##TYPE() { \
            struct _list__list_##TYPE* list = \
                (struct _list__list_##TYPE*)malloc( \
                    sizeof(struct _list__list_##TYPE)); \
            if (!list) { \
                return NULL; \
            } \
            list->head = NULL; \
            list->tail = NULL; \
            list->length = 0; \
            return list; \
        } \
\
        static bool _list__clear_##TYPE( \
                struct _list__list_##TYPE* list); \
\
        static bool _list__destroy_##TYPE( \
                struct _list__list_##TYPE* list) { \
            if (!list) { \
                return true; \
            } \
            if (!_list__clear_##TYPE(list)) { \
                return false; \
            } \
            free(list); \
            return true; \
        } \
\
        static bool _list__push_back_##TYPE( \
                struct _list__list_##TYPE* list, \
                TYPE value) { \
            if (!list) { \
                return false; \
            } \
            struct _list__node_##TYPE* node = \
                (struct _list__node_##TYPE*)malloc( \
                    sizeof(struct _list__node_##TYPE)); \
            if (!node) { \
                return false; \
            } \
            node->value = value; \
            node->prev = list->tail; \
            node->next = NULL; \
            list->tail = node; \
            if (node->prev) { \
                node->prev->next = node; \
            } \
            else { \
                list->head = node; \
            } \
            ++list->length; \
            return true; \
        } \
\
        static bool _list__insert_##TYPE( \
                struct _list__iterator_##TYPE iter, \
                TYPE value) { \
            if (!iter.list) { \
                return false; \
            } \
            struct _list__node_##TYPE* node = \
                (struct _list__node_##TYPE*)malloc( \
                    sizeof(struct _list__node_##TYPE)); \
            if (!node) { \
                return false; \
            } \
            node->value = value; \
            node->next = iter.current; \
            if (node->next) { \
                node->prev = node->next->prev; \
                if (node->prev) { \
                    node->prev->next = node; \
                } \
                node->next->prev = node; \
            } \
            else { \
                node->prev = iter.list->tail; \
                if (node->prev) { \
                    node->prev->next = node; \
                } \
                iter.list->tail = node; \
            } \
            if (!node->prev) { \
                iter.list->head = node; \
            } \
            ++iter.list->length; \
            return true; \
        } \
\
        static bool _list__clear_##TYPE( \
                struct _list__list_##TYPE* list) { \
            if (!list || !list->length || !list->head) { \
                assert(!(list && list->tail)); \
                return true; \
            } \
            struct _list__node_##TYPE* temp_node; \
            while (list->head) { \
                temp_node = list->head->next; \
                free(list->head); \
                list->head = temp_node; \
            } \
            list->head = NULL; \
            list->tail = NULL; \
            list->length = 0; \
            return true; \
        } \
\
        static bool _list__copy_##TYPE( \
                const struct _list__list_##TYPE* source_list, \
                struct _list__list_##TYPE* destination_list) { \
            if (!source_list || !destination_list) { \
                return false; \
            } \
            for (struct _list__node_##TYPE* node = source_list->head; \
                    node; \
                    node = node->next) { \
                if (!_list__push_back_##TYPE(destination_list, node->value)) { \
                    return false; \
                } \
            } \
            return true; \
        } \
\
        static bool _list__move_##TYPE( \
                struct _list__list_##TYPE* source_list, \
                struct _list__list_##TYPE* destination_list) { \
            if (!source_list || !destination_list) { \
                return false; \
            } \
            if (destination_list->tail) { \
                destination_list->tail->next = source_list->head; \
                source_list->head->prev = destination_list->tail; \
                destination_list->tail = source_list->tail; \
            } \
            else { \
                destination_list->head = source_list->head; \
                destination_list->tail = source_list->tail; \
            } \
            destination_list->length += source_list->length; \
            source_list->head = NULL; \
            source_list->tail = NULL; \
            source_list->length = 0; \
            return true; \
        } \
\
        static struct _list__iterator_##TYPE \
        _list__find_by_value_cmp_##TYPE( \
                struct _list__list_##TYPE* list, \
                TYPE* value, \
                int (*comparator)(TYPE* a, TYPE* b)) { \
            struct _list__iterator_##TYPE iter = { NULL, list }; \
            if (!list || !list->length) { \
                return iter; \
            } \
            iter.current = list->head; \
            while (iter.current) { \
                if (comparator(&iter.current->value, value) == 0) { \
                    break; \
                } \
                iter.current = iter.current->next; \
            } \
            return iter; \
        } \
\
        static bool _list__remove_##TYPE( \
                struct _list__iterator_##TYPE iter) { \
            if (!iter.list || !iter.current) { \
                return false; \
            } \
            if (iter.current->next) { \
                iter.current->next->prev = iter.current->prev; \
            } \
            else { \
                iter.list->tail = iter.current->prev; \
            } \
            if (iter.current->prev) { \
                iter.current->prev->next = iter.current->next; \
            } \
            else { \
                iter.list->head = iter.current->next; \
            } \
            free(iter.current); \
            --iter.list->length; \
            return true; \
        } \
\
        static bool _list__remove_by_value_cmp_##TYPE( \
                struct _list__list_##TYPE* list, \
                TYPE* value, \
                int (*comparator)(TYPE* a, TYPE* b)) { \
            struct _list__iterator_##TYPE iter = \
                _list__find_by_value_cmp_##TYPE(list, value, comparator); \
            return _list__remove_##TYPE(iter); \
        } \
\
        static TYPE _list__pop_front_##TYPE( \
                struct _list__list_##TYPE* list) { \
            assert(list->head); \
            if (list->head->next) { \
                list->head->next->prev = NULL; \
            } \
            else { \
                list->tail = NULL; \
            } \
            _list__node_##TYPE* node = list->head; \
            list->head = list->head->next; \
            TYPE value = node->value; \
            free(node); \
            --list->length; \
            return value; \
        } \
\
        static TYPE _list__pop_back_##TYPE( \
                struct _list__list_##TYPE* list) { \
            assert(list->tail); \
            if (list->tail->prev) { \
                list->tail->prev->next = NULL; \
            } \
            else { \
                list->head = NULL; \
            } \
            _list__node_##TYPE* node = list->tail; \
            list->tail = list->tail->prev; \
            TYPE value = node->value; \
            free(node); \
            --list->length; \
            return value; \
        } \
\
        static struct _list__iterator_##TYPE \
        _list__begin_##TYPE( \
                struct _list__list_##TYPE* list) { \
            assert(list); \
            struct _list__iterator_##TYPE iter = { list->head, list }; \
            return iter; \
        } \
\
        static struct _list__iterator_##TYPE \
        _list__end_##TYPE( \
                struct _list__list_##TYPE* list) { \
            assert(list); \
            struct _list__iterator_##TYPE iter = { NULL, list }; \
            return iter; \
        } \
\
        static struct _list__iterator_##TYPE \
        _list__tail_##TYPE( \
                struct _list__list_##TYPE* list) { \
            assert(list); \
            struct _list__iterator_##TYPE iter = { list->tail, list }; \
            return iter; \
        } \
\
        static void _list__next_##TYPE( \
                struct _list__iterator_##TYPE* iter) { \
            if (iter && iter->current) { \
                iter->current = iter->current->next; \
            } \
        } \
\
        static void _list__prev_##TYPE( \
                struct _list__iterator_##TYPE* iter) { \
            if (iter && iter->current) { \
                iter->current = iter->current->prev; \
            } \
        }

#define DEFINE_LIST_OPS_FOR_BUILTINS(TYPE) \
\
        static struct _list__iterator_##TYPE \
        _list__find_by_value_##TYPE( \
                struct _list__list_##TYPE* list, \
                TYPE value) { \
            struct _list__iterator_##TYPE iter = { NULL, list }; \
            if (!list || !list->length) { \
                return iter; \
            } \
            iter.current = list->head; \
            while (iter.current) { \
                if (iter.current->value == value) { \
                    break; \
                } \
                iter.current = iter.current->next; \
            } \
            return iter; \
        } \
\
        static bool _list__remove_by_value_##TYPE( \
                struct _list__list_##TYPE* list, \
                TYPE value) { \
            struct _list__iterator_##TYPE iter = \
                _list__find_by_value_##TYPE(list, value); \
            return _list__remove_##TYPE(iter); \
        }

#define LIST_TYPE(TYPE) struct _list__list_##TYPE*
#define LIST_ITER_TYPE(TYPE) struct _list__iterator_##TYPE

#define list_create(TYPE) _list__create_##TYPE()
#define list_destroy(TYPE, list) _list__destroy_##TYPE(list)
#define list_push_back(TYPE, list, value) _list__push_back_##TYPE(list, value)
#define list_insert(TYPE, iter, value) _list__insert_##TYPE(iter, value)
#define list_clear(TYPE, list) _list__clear_##TYPE(list)
#define list_copy(TYPE, source_list, destination_list) \
    _list__copy_##TYPE(source_list, destination_list)
#define list_move(TYPE, source_list, destination_list) \
    _list__move_##TYPE(source_list, destination_list)
#define list_find_by_value(TYPE, list, value) \
    _list__find_by_value_##TYPE(list, value)
#define list_find_by_value_cmp(TYPE, list, value, comparator) \
    _list__find_by_value_cmp_##TYPE(list, &value, &comparator)
#define list_remove(TYPE, iter) _list__remove_##TYPE(iter)
#define list_remove_by_value(TYPE, list, value) \
    _list__remove_by_value_##TYPE(list, value)
#define list_remove_by_value_cmp(TYPE, list, value, comparator) \
    _list__remove_by_value_cmp_##TYPE(list, &value, &comparator)
#define list_pop_front(TYPE, list) _list__pop_front_##TYPE(list)
#define list_pop_back(TYPE, list) _list__pop_back_##TYPE(list)

#define list_len(list) list->length
#define list_empty(list) (list->length == 0)

#define list_begin(TYPE, list) _list__begin_##TYPE(list)
#define list_end(TYPE, list) _list__end_##TYPE(list)
#define list_head(TYPE, list) list_begin(TYPE, list)
#define list_tail(TYPE, list) _list__tail_##TYPE(list)
#define list_first(list) list->head->value
#define list_last(list) list->tail->value
#define list_next(TYPE, iter) _list__next_##TYPE(&iter)
#define list_prev(TYPE, iter) _list__prev_##TYPE(&iter)
#define list_iter_valid(iter) (!!iter.current)
#define list_iter_value(iter) iter.current->value
#define list_iter_eq(iter_i, iter_j) (iter_i.current == iter_j.current)

#define list_for_each(TYPE, list, var_name) \
    assert(list); \
    for (struct _list__node_##TYPE* var_name = list->head; \
            var_name; \
            var_name = var_name->next)
#define list_for_each_reversed(TYPE, list, var_name) \
    assert(list); \
    for (struct _list__node_##TYPE* var_name = list->tail; \
            var_name; \
            var_name = var_name->prev)
#define list_var_value(var_name) var_name->value
#define list_var_eq(var_name1, var_name2) (var_name1 == var_name2)

#endif //CLIST_LIST_H
