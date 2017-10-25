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
        struct _list__list_iterator_##TYPE { \
            struct _list__node_##TYPE* current; \
            struct _list__list_##TYPE* list; \
        }; \
\
        struct _list__list_##TYPE* _list__create_list_##TYPE() { \
            struct _list__list_##TYPE* list = \
                (struct _list__list_##TYPE*)malloc(sizeof(struct _list__list_##TYPE)); \
            if (!list) { \
                return NULL; \
            } \
            list->head = NULL; \
            list->tail = NULL; \
            list->length = 0; \
            return list; \
        } \
\
        bool _list__clear_list_##TYPE(struct _list__list_##TYPE* list); \
\
        bool _list__destroy_list_##TYPE(struct _list__list_##TYPE* list) { \
            if (!list) { \
                return true; \
            } \
            if (!_list__clear_list_##TYPE(list)) { \
                return false; \
            } \
            free(list); \
            return true; \
        } \
\
        bool _list__push_back_##TYPE(struct _list__list_##TYPE* list, TYPE value) { \
            if (!list) { \
                return false; \
            } \
            struct _list__node_##TYPE* node = \
                (struct _list__node_##TYPE*)malloc(sizeof(struct _list__node_##TYPE)); \
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
        bool _list__insert_##TYPE(struct _list__list_iterator_##TYPE iter, TYPE value) { \
            if (!iter.list) { \
                return false; \
            } \
            struct _list__node_##TYPE* node = \
                (struct _list__node_##TYPE*)malloc(sizeof(struct _list__node_##TYPE)); \
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
        bool _list__clear_list_##TYPE(struct _list__list_##TYPE* list) { \
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
        bool _list__copy_list_##TYPE(const struct _list__list_##TYPE* source_list, \
                                     struct _list__list_##TYPE* destination_list) { \
            if (!source_list || !destination_list) { \
                return false; \
            } \
            for (struct _list__node_##TYPE* node = source_list->head; node; node = node->next) { \
                if (!_list__push_back_##TYPE(destination_list, node->value)) { \
                    return false; \
                } \
            } \
            return true; \
        } \
\
        bool _list__move_list_##TYPE(struct _list__list_##TYPE* source_list, \
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
        struct _list__list_iterator_##TYPE _list__find_by_value_##TYPE( \
                struct _list__list_##TYPE* list, \
                TYPE value) { \
            struct _list__list_iterator_##TYPE iter = { NULL, list }; \
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
        bool _list__remove_##TYPE(struct _list__list_iterator_##TYPE iter) { \
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
        bool _list__remove_by_value_##TYPE(struct _list__list_##TYPE* list, TYPE value) { \
            struct _list__list_iterator_##TYPE iter = _list__find_by_value_##TYPE(list, value); \
            return _list__remove_##TYPE(iter); \
        } \
\
        struct _list__list_iterator_##TYPE _list__begin_##TYPE(struct _list__list_##TYPE* list) { \
            struct _list__list_iterator_##TYPE iter = { list->head, list }; \
            return iter; \
        } \
\
        struct _list__list_iterator_##TYPE _list__end_##TYPE(struct _list__list_##TYPE* list) { \
            struct _list__list_iterator_##TYPE iter = { NULL, list }; \
            return iter; \
        } \
\
        void _list__next_##TYPE(struct _list__list_iterator_##TYPE* iter) { \
            if (iter && iter->current) { \
                iter->current = iter->current->next; \
            } \
        } \
\
        void _list__prev_##TYPE(struct _list__list_iterator_##TYPE* iter) { \
            if (iter && iter->current) { \
                iter->current = iter->current->prev; \
            } \
        }

#define LIST_TYPE(TYPE) struct _list__list_##TYPE*
#define LIST_ITER_TYPE(TYPE) struct _list__list_iterator_##TYPE

#define create_list(TYPE) _list__create_list_##TYPE()
#define destroy_list(TYPE, list) _list__destroy_list_##TYPE(list)
#define push_back(TYPE, list, value) _list__push_back_##TYPE(list, value)
#define insert(TYPE, iter, value) _list__insert_##TYPE(iter, value)
#define clear_list(TYPE, list) _list__clear_list_##TYPE(list)
#define copy_list(TYPE, source_list, destination_list) \
    _list__copy_list_##TYPE(source_list, destination_list)
#define move_list(TYPE, source_list, destination_list) \
    _list__move_list_##TYPE(source_list, destination_list)
#define find_by_value(TYPE, list, value) _list__find_by_value_##TYPE(list, value)
#define remove(TYPE, iter) _list__remove_##TYPE(iter)
#define remove_by_value(TYPE, list, value) _list__remove_by_value_##TYPE(list, value)

#define list_len(list) list->length

#define begin(TYPE, list) _list__begin_##TYPE(list)
#define end(TYPE, list) _list__end_##TYPE(list)
#define next(TYPE, iter) _list__next_##TYPE(&iter)
#define prev(TYPE, iter) _list__prev_##TYPE(&iter)
#define iter_valid(iter) (!!iter.current)
#define iter_value(iter) iter.current->value

#define for_each_in_list(TYPE, list, var_name) \
    for (struct _list__node_##TYPE* var_name = list->head; var_name; var_name = var_name->next)
#define for_each_in_list_reversed(TYPE, list, var_name) \
    for (struct _list__node_##TYPE* var_name = list->tail; var_name; var_name = var_name->prev)
#define var_value(var_name) var_name->value

#endif //CLIST_LIST_H
