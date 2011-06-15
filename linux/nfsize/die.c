/*
 * die.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "nfsize.h"

void die(const char *what, ...)
{
    va_list args;
    va_start(args, what);
    fprintf(stderr, "%s: error: ", prog);
    vfprintf(stderr, what, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(EXIT_FAILURE);
}

