/*
 * $Id: benchmark.c,v 0.1 2012/01/13 12:39:53 dankogai Exp dankogai $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define EXIT(msg) { perror(msg); exit(errno); }
#define LINESIZE 65536

#include "bucketsort.h"
#include "bucketsort.c"

inline int radix(void *v, size_t d)
{
    long n = (long) v;
    long r = (n >> ((sizeof(long) - d - 1) * 8)) & 0xff;
    return r;
}

inline int cmp(void *a, void *b)
{
    return *(long *) a - *(long *) b;
}

inline int dcmp(void *a, void *b)
{
    return a - b;
}

int main(int argc, char **argv)
{
    if (argc < 2)
	EXIT("no file");
    FILE *fp = fopen(argv[1], "r");
    if (!fp)
	EXIT(argv[1]);
    char buf[LINESIZE];
    size_t ncur, nlen = LINESIZE;
    long *numbers = (long *) malloc(nlen * sizeof(long));
    if (!numbers)
	EXIT("malloc failed");
    for (ncur = 0; fgets(buf, LINESIZE, fp); ncur++) {
	if (ncur == nlen) {
	    nlen <<= 1;
	    numbers = realloc(numbers, nlen * sizeof(long));
	    if (!numbers)
		EXIT("malloc failed");
	}
	numbers[ncur] = atoi(buf);
    }
    size_t i;
#if defined(QSORT)
    qsort(numbers, ncur, sizeof(long), cmp);
#elif defined(MERGESORT)
    mergesort(numbers, ncur, sizeof(long), cmp);
#elif defined(HEAPSORT)
    heapsort(numbers, ncur, sizeof(long), cmp);
#elif defined(BUCKETSORT)
bucketsort((void **) numbers, ncur, NULL, radix, dcmp);
#else
#endif
    for (i = 0; i < ncur; i++)
	printf("%ld\n", numbers[i]);

    free(numbers);
    return 0;
}
