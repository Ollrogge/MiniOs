#include <list.h>

static list_node_t *_list_sort(list_node_t *list, list_cmp_func_t cmp);

bool list_is_empty(const list_node_t *list)
{
    return list->next == NULL;
}

/**
 * @brief Appends *new_node* at the end of *list
 *
 * @note Complexity: O(1)
 *
 * @param[in,out]   list        Pointer to clist
 * @param[in,out]   new_node    Node which gets inserted.
 *                              Must not be NULL.
 */

void list_rpush(list_node_t *list, list_node_t *new_node)
{
    if (list->next) {
        new_node->next = list->next->next;
        list->next->next = new_node;
    }
    else {
        new_node->next = new_node;
    }
    list->next = new_node;
}

/**
 * @brief Inserts *new_node* at the beginning of *list
 *
 * @note Complexity: O(1)
 *
 * @param[in,out]   list        Pointer to clist
 * @param[in,out]   new_node    Node which gets inserted.
 *                              Must not be NULL.
 */
void list_lpush(list_node_t *list, list_node_t *new_node)
{
    if (list->next) {
        new_node->next = list->next->next;
        list->next->next = new_node;
    }
    else {
        new_node->next = new_node;
        list->next = new_node;
    }
}

/**
 * @brief Removes and returns first element from list
 *
 * @note Complexity: O(1)
 *
 * @param[in,out]   list        Pointer to the *list* to remove first element
 *                              from.
 */
list_node_t *list_lpop(list_node_t *list)
{
    if (list->next) {
        list_node_t *first = list->next->next;
        if (list->next == first) {
            list->next = NULL;
        }
        else {
            list->next->next = first->next;
        }
        return first;
    }
    else {
        return NULL;
    }
}

/**
 * @brief Advances the circle list.
 *
 * The result of this function is will be a list with
 * nodes shifted by one. So second list entry will be
 * first, first is last.
 *
 * [ A, B, C ] becomes [ B, C, A ]
 *
 * @note Complexity: O(1)
 *
 * @param[in,out]   list        The list to work upon.
 */
void list_lpoprpush(list_node_t *list)
{
    if (list->next) {
        list->next = list->next->next;
    }
}

/**
 * @brief Returns first element in list
 *
 * @note: Complexity: O(1)
 *
 * @param[in]   list        The list to work upon.
 * @returns     first (leftmost) list element, or NULL if list is empty
 */
list_node_t *list_lpeek(const list_node_t *list)
{
    if (list->next) {
        return list->next->next;
    }
    return NULL;
}

/**
 * @brief Returns last element in list
 *
 * @note: Complexity: O(1)
 *
 * @param[in]   list        The list to work upon.
 * @returns     last (rightmost) list element, or NULL if list is empty
 */
list_node_t *list_rpeek(const list_node_t *list)
{
    return list->next;
}

/**
 * @brief Removes and returns last element from list
 *
 * @note Complexity: O(n) with n being the number of elements in the list.
 *
 * @param[in,out]   list        Pointer to the *list* to remove last element
 *                              from.
 */
list_node_t *list_rpop(list_node_t *list)
{
    if (list->next) {
        list_node_t *last = list->next;
        while (list->next->next != last) {
            list_lpoprpush(list);
        }
        return list_lpop(list);
    }
    else {
        return NULL;
    }
}

/**
 * @brief Finds node and returns its predecessor
 *
 * @note Complexity: O(n)
 *
 * @param[in]       list    pointer to clist
 * @param[in,out]   node    Node to look for
 *                          Must not be NULL.
 *
 * @returns         predecessor of node if found
 * @returns         NULL if node is not a list member
 */
list_node_t *list_find_before(const list_node_t *list,
                                              const list_node_t *node)
{
    list_node_t *pos = list->next;

    if (!pos) {
        return NULL;
    }
    do {
        pos = pos->next;
        if (pos->next == node) {
            return pos;
        }
    } while (pos != list->next);

    return NULL;
}

/**
 * @brief Finds and returns node
 *
 * @note Complexity: O(n)
 *
 * @param[in]       list    pointer to clist
 * @param[in,out]   node    Node to look for
 *                          Must not be NULL.
 *
 * @returns         node if found
 * @returns         NULL if node is not a list member
 */
list_node_t *list_find(const list_node_t *list,
                                       const list_node_t *node)
{
    list_node_t *tmp = list_find_before(list, node);

    if (tmp) {
        return tmp->next;
    }
    else {
        return NULL;
    }
}

/**
 * @brief Finds and removes node
 *
 * @note Complexity: O(n)
 *
 * @param[in]       list    pointer to clist
 * @param[in,out]   node    Node to remove for
 *                          Must not be NULL.
 *
 * @returns         node if found and removed
 * @returns         NULL if node is not a list member
 */
list_node_t *list_remove(list_node_t *list, list_node_t *node)
{
    if (list->next) {
        if (list->next->next == node) {
            return list_lpop(list);
        }
        else {
            list_node_t *tmp = list_find_before(list, node);
            if (tmp) {
                tmp->next = tmp->next->next;
                if (node == list->next) {
                    list->next = tmp;
                }
                return node;
            }
        }
    }

    return NULL;
}

/**
 * @brief Traverse clist, call function for each member
 *
 * The pointer supplied by @p arg will be passed to every call to @p func.
 *
 * If @p func returns non-zero, traversal will be aborted like when calling
 * break within a for loop, returning the corresponding node.
 *
 * @param[in]       list        List to traverse.
 * @param[in]       func        Function to call for each member.
 * @param[in]       arg         Pointer to pass to every call to @p func
 *
 * @returns         NULL on empty list or full traversal
 * @returns         node that caused @p func(node, arg) to exit non-zero
 */
list_node_t *list_foreach(list_node_t *list, int (*func)(
                                              list_node_t *,
                                              void *), void *arg)
{
    list_node_t *node = list->next;

    if (node) {
        do {
            node = node->next;
            if (func(node, arg)) {
                return node;
            }
        } while (node != list->next);
    }

    return NULL;
}

/**
 * @brief Typedef for comparison function used by @ref clist_sort()
 *
 */
typedef int (*list_cmp_func_t)(list_node_t *a, list_node_t *b);

/**
 * @brief   List sorting helper function
 *
 * @internal
 *
 * @param[in]   list_head   ptr to the first element inside a clist
 * @param[in]   cmp         comparison function
 *
 * @returns     ptr to *last* element in list
 */
list_node_t *_list_sort(list_node_t *list_head, list_cmp_func_t cmp);

/**
 * @brief   Sort a list
 *
 * This function will sort @p list using merge sort.
 * The sorting algorithm runs in O(N log N) time. It is also stable.
 *
 * Apart from the to-be-sorted list, the function needs a comparison function.
 * That function will be called by the sorting implementation for every
 * comparison.  It gets two pointers a, b of type "clist_node_t" as parameters
 * and must return
 * <0, 0 or >0 if a is lesser, equal or larger than b.
 *
 * Example:
 *
 *     typedef struct {
 *         clist_node_t next;
 *         uint32_t value;
 *     } mylist_node_t;
 *
 *     int _cmp(clist_node_t *a, clist_node_t *b)
 *     {
 *         uint32_t a_val = ((mylist_node_t *)a)->value;
 *         uint32_t b_val = ((mylist_node_t *)b)->value;
 *
 *         if (a_val < b_val) {
 *             return -1;
 *         }
 *         else if (a_val > b_val) {
 *             return 1;
 *         }
 *         else {
 *             return 0;
 *         }
 *     }
 *
 *     ...
 *
 *     clist_sort(list, _cmp);
 *
 * @param[in,out]   list    List to sort
 * @param[in]       cmp     Comparison function
 */
void list_sort(list_node_t *list, list_cmp_func_t cmp)
{
    if (list->next) {
        list->next = _list_sort(list->next->next, cmp);
    }
}

/**
 * @brief   Count the number of items in the given list
 *
 * @param[in]   list    ptr to the clist
 *
 * @return  the number of elements in the given list
 */
size_t list_count(list_node_t *list)
{
    list_node_t *node = list->next;
    size_t cnt = 0;

    if (node) {
        do {
            node = node->next;
            ++cnt;
        } while (node != list->next);
    }

    return cnt;
}

/**
 * @brief   Tells if a list has exactly one element
 *
 * @note    Complexity: O(1)
 *
 * @param[in]   list    Pointer to the clist
 *
 * @retval      true    If list has exactly one element
 */
bool list_exactly_one(list_node_t *list)
{
    return !list_is_empty(list) && (list->next == list->next->next);
}

/**
 * @brief   Tells if a list has more than one element
 *
 * @note    Complexity: O(1)
 *
 * @param[in]   list    Pointer to the clist
 *
 * @retval      true    If list has more than one element
 */
bool list_more_than_one(list_node_t *list)
{
    return !list_is_empty(list) && (list->next != list->next->next);
}

static list_node_t *_list_sort(list_node_t *list, list_cmp_func_t cmp)
{
    list_node_t *p, *q, *e;
    int insize, psize, qsize, i;

    /*
     * Silly special case: if `list' was passed in as NULL, return
     * NULL immediately.
     */
    if (!list) {
        return NULL;
    }

    insize = 1;

    while (1) {
        list_node_t *tail = NULL;
        list_node_t *oldhead = list;
        p = list;
        list = NULL;

        int nmerges = 0;  /* count number of merges we do in this pass */

        while (p) {
            nmerges++;  /* there exists a merge to be done */
            /* step `insize' places along from p */
            q = p;
            psize = 0;
            for (i = 0; i < insize; i++) {
                psize++;
                q = (q->next == oldhead) ? NULL : q->next;
                /* cppcheck-suppress nullPointer
                 * (reason: possible bug in cppcheck 1.6x) */
                if (!q) {
                    break;
                }
            }

            /* if q hasn't fallen off end, we have two lists to merge */
            qsize = insize;

            /* now we have two lists; merge them */
            while (psize > 0 || (qsize > 0 && q)) {

                /* decide whether next element of merge comes from p or q */
                if (psize == 0) {
                    /* p is empty; e must come from q. */
                    e = q; q = q->next; qsize--;
                    if (q == oldhead) {
                        q = NULL;
                    }
                }
                else if (qsize == 0 || !q) {
                    /* q is empty; e must come from p. */
                    e = p; p = p->next; psize--;
                    if (p == oldhead) {
                        p = NULL;
                    }
                }
                else if (cmp(p, q) <= 0) {
                    /* First element of p is lower (or same);
                     * e must come from p. */
                    e = p; p = p->next; psize--;
                    if (p == oldhead) {
                        p = NULL;
                    }
                }
                else {
                    /* First element of q is lower; e must come from q. */
                    e = q; q = q->next; qsize--;
                    if (q == oldhead) {
                        q = NULL;
                    }
                }

                /* add the next element to the merged list */
                if (tail) {
                    tail->next = e;
                }
                else {
                    list = e;
                }
                tail = e;
            }

            /* now p has stepped `insize' places along, and q has too */
            p = q;
        }

        /* cppcheck-suppress nullPointer
         * (reason: tail cannot be NULL at this point, because list != NULL) */
        tail->next = list;

        /* If we have done only one merge, we're finished. */
        if (nmerges <= 1) { /* allow for nmerges==0, the empty list case */
            return tail;
        }

        /* Otherwise repeat, merging lists twice the size */
        insize *= 2;
    }
}