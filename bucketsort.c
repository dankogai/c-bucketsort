/*
 * $Id: bucketsort.c,v 0.1 2012/01/12 21:52:11 dankogai Exp dankogai $
 *
 *  Licensed under the MIT license.
 *  http://www.opensource.org/licenses/mit-license.php
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#define BUCKETSIZ 256

#include "bucketsort.h"

inline list_t shift(list_t * src)
{
    list_t dst = *src;
    *src = (*src)->cdr;
    dst->cdr = NULL;
    return dst;
}

inline void enqueue(_queue_t * q, list_t l, list_t t)
{
    if (q->head) {
	q->tail->cdr = l;
	q->tail = l;
    } else {
	q->head = q->tail = l;
    }
    if (t)
	q->tail = t;
};

inline list_t dequeue(_queue_t * q)
{
    list_t l = q->head;
    q->head = q->head->cdr;
    l->cdr = NULL;
    return l;
};

inline int charAt(void *v, size_t i)
{
    char *s = (char *) v;
    return s ? strlen(s) > i ? s[i] : 0 : 0;
};

inline void *identity(void *v)
{
    return v;
}

inline int allthesame(_queue_t q, comparator_t cmp)
{
    if (q.head == q.tail)
	return 1;
    char *key = q.head->car;
    list_t l;
    for (l = q.head; l; l = l->cdr) {
	if (cmp(key, l->car) != 0)
	    return 0;
    }
    return 1;
}

inline _queue_t _bucketsort_q(_queue_t src, size_t depth,
			      keyaccessor_t key, indexer_t idx,
			      comparator_t cmp)
{
    list_t head = src.head;
    if (!head)
	return src;
    if (!key)
	key = identity;
    if (!idx)
	idx = charAt;
    if (!cmp)
	cmp = (comparator_t) strcmp;
    _queue_t buckets[BUCKETSIZ];
    size_t i;
    for (i = 0; i < BUCKETSIZ; i++)
	buckets[i].head = NULL;
    while (head) {
	list_t l = shift(&head);
	int c = idx(key(l->car), depth);
	enqueue(&buckets[c], l, NULL);
    }
    _queue_t dst = { NULL, NULL };
    for (i = 0; i < BUCKETSIZ; i++) {
	_queue_t b = buckets[i];
	if (b.head) {
	    if (!allthesame(b, cmp))
		b = _bucketsort_q(b, depth + 1, key, idx, cmp);
	    enqueue(&dst, b.head, b.tail);
	}
    }
    return dst;
};

list_t bucketsort_l(list_t src, keyaccessor_t key, indexer_t idx,
		    comparator_t cmp)
{
    _queue_t q = { src, NULL };
    return _bucketsort_q(q, 0, key, idx, cmp).head;
}

inline list_t array2list(void *a[], _cons_t lists[], size_t n)
{
    size_t i;
    list_t l;
    for (i = 0, l = lists; i < n; i++) {
	lists[i].car = a[i];
	lists[i].cdr = ++l;
    }
    lists[--i].cdr = NULL;
    return lists;
}

inline void **list2array(list_t l, void **a, size_t n)
{
    size_t i;
    for (i = 0; i < n && l; i++, l = l->cdr)
	a[i] = l->car;
    return a;
}

int bucketsort(void **a, size_t n, keyaccessor_t key, indexer_t idx,
	       comparator_t cmp)
{
    _cons_t *lists = (_cons_t *) malloc(n * sizeof(_cons_t));
    if (!lists) {
	perror("malloc failed!");
	return -1;
    }
    array2list(a, lists, n);
    list_t l = bucketsort_l(lists, key, idx, cmp);
    list2array(l, a, n);
    free(lists);
    return 0;
}

#if 0
int main(int argc, char **argv)
{
    if (!argc)
	return 0;
    bucketsort((void **) argv, argc);
    int i;
    for (i = 0; i < argc; i++) {
	printf("%s\n", argv[i]);
    };
};
#endif
