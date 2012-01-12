/*
 * $Id: bucketsort.h,v 0.1 2012/01/12 21:52:11 dankogai Exp dankogai $
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

list_t bucketsort_l(list_t, keyaccessor_t, indexer_t, comparator_t);
int bucketsort(void **, size_t, keyaccessor_t, indexer_t, comparator_t);

#endif
