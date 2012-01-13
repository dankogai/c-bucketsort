#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define EXIT(msg) { perror(msg); exit(errno); }
#define LINESIZE 65536

#include "bucketsort.h"

#ifdef TEST_NUMERIC
void *asis(void *v)
{
    return v;
}

int radix(void *v, size_t d)
{
    long n = (long) v;
    long r = (n >> ((sizeof(long) - d - 1) * 8)) & 0xff;
    return r;
}

int cmp(void *a, void *b)
{
    return a - b;
}

int main(int argc, char **argv)
{
    if (argc < 2)
	EXIT("number?");
    long i, n = atoi(argv[1]);
    long *numbers = (long *) malloc(n * sizeof(long));
    if (!numbers)
	EXIT("malloc failed");
    srandom(time(NULL));
    for (i = 0; i < n; i++)
	numbers[i] = random();
    printf("# before\n");
    for (i = 0; i < n; i++)
	printf("%ld\n", numbers[i]);
    printf("# after\n");
    bucketsort((void **) numbers, n, asis, radix, cmp);
    for (i = 0; i < n; i++)
	printf("%ld\n", numbers[i]);
    free(numbers);
    return 0;
};

#else

int main(int argc, char **argv)
{
    if (argc < 2)
	EXIT("no file");
    FILE *fp = fopen(argv[1], "r");
    if (!fp)
	EXIT(argv[1]);
    char buf[LINESIZE];
    size_t lcur, llen = LINESIZE;
    char **lines = (char **) malloc(llen * sizeof(char *));
    if (!lines)
	EXIT("malloc failed");
    for (lcur = 0; fgets(buf, LINESIZE, fp); lcur++) {
	if (lcur == llen) {
	    llen <<= 1;
	    // fprintf(stderr, "realloc %d bytes\n", llen * sizeof(char *));
	    lines = realloc(lines, llen * sizeof(char *));
	    if (!lines)
		EXIT("malloc failed");
	}
	int slen = strlen(buf);
	if (slen > 0 && buf[slen - 1] == '\n')
	    buf[slen - 1] = '\0';	// do not copy \n
	lines[lcur] = strdup(buf);
	if (!lines[lcur])
	    EXIT("malloc failed");
    }
    size_t i;
    // for (i = 0; i < lcur; i++) printf("%s\n", lines[i]);
    bucketsort((void **) lines, lcur, NULL, NULL, NULL);
    for (i = 0; i < lcur; i++)
	printf("%s\n", lines[i]);
    {
	for (i = 0; i < lcur; i++)
	    free(lines[i]);
	free(lines);
    }
    return 0;
}
#endif
