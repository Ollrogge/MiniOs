#ifndef LIST_H
#define LIST_H

#include <no.h>

typedef struct list_node {
    struct list_node *next;     /**< pointer to next list entry */
} list_node_t;

void list_rpush(list_node_t *list, list_node_t *new_node);

void list_lpush(list_node_t *list, list_node_t *new_node);

list_node_t *list_lpop(list_node_t *list);

void list_lpoprpush(list_node_t *list);

list_node_t *list_lpeek(const list_node_t *list);

list_node_t *list_rpeek(const list_node_t *list);

list_node_t *list_rpop(list_node_t *list);

list_node_t *list_find_before(const list_node_t *list,const list_node_t *node);

list_node_t *list_find(const list_node_t *list,
const list_node_t *node);

list_node_t *list_remove(list_node_t *list, list_node_t *node);

list_node_t *list_foreach(list_node_t *list, int (*func)(list_node_t *, void *), void *arg);

typedef int (*list_cmp_func_t)(list_node_t *a, list_node_t *b);

void list_sort(list_node_t *list, list_cmp_func_t cmp);

size_t list_count(list_node_t *list);

bool list_exactly_one(list_node_t *list);

bool list_more_than_one(list_node_t *list);

bool list_is_empty(const list_node_t *list);

#endif /* LIST_H */
