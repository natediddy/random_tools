/*
 * file.c
 */

#include <stdio.h>
#include "nfsize.h"

void is_file(nfselem **nfs)
{
    FILE *file = fopen((*nfs)->name,"r");
    
    if (file == NULL) {
        return;
    }
    (*nfs)->f_def = true;
    if (fclose(file) != 0) {
        die("is_file(): fclose()");
    }
}

void f_getbytes(nfselem **nfs)
{
    int ch;
    FILE *file = NULL;

    if (!(*nfs)->f_def) {
        return;
    }
    file = fopen((*nfs)->name,"r");
    while ((ch = fgetc(file)) != EOF) {
        (*nfs)->bytes++;
    }
    if (fclose(file) != 0) {
        die("f_getbytes(): fclose()");
    }
}

