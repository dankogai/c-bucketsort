/*
 * $Id: bucketsort.h,v 0.2 2012/01/13 09:59:13 dankogai Exp $
 *  Licensed under the MIT license.
 *  http://www.opensource.org/licenses/mit-license.php
 *
 */

#ifndef _BUCKETSORT_H_
#define _BUCKETSORT_H_

typedef struct {
    void *car, *cdr;
} _cons_t, *list_t;

typedef struct {
    list_t head, tail;
} _queue_t;

typedef void *(*keyaccessor_t) (void *);
typedef int (*indexer_t) (void *, size_t);
typedef int (*comparator_t) (void *, void *);

list_t array2list(void **, _cons_t *, size_t);
void **list2array(list_t, void **, size_t);
list_t mergesort_l(list_t, comparator_t);
list_t bucketsort_l(list_t, keyaccessor_t, indexer_t, comparator_t);
int bucketsort(void **, size_t, keyaccessor_t, indexer_t, comparator_t);

#define BUCKETSORT_DEPTH 256

#endif
