/*
 * file.c
 */

#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "nfsize.h"

void is_file(nfselem **nfs)
{
    struct stat statbuf;
    
    memset(&statbuf, 0, sizeof(struct stat));
    if (!stat((*nfs)->name, &statbuf)) {
        if (S_ISREG(statbuf.st_mode)) {
            (*nfs)->f_def = true;
        }
    } else {
        die("is_file(): %s\n", strerror(errno));
    }
}

void f_getbytes(nfselem **nfs)
{
    struct stat statbuf;

    memset(&statbuf, 0, sizeof(struct stat));
    if (!stat((*nfs)->name, &statbuf)) {
        (*nfs)->bytes = (unsigned int)statbuf.st_size;
    } else {
        die("f_getbytes(): %s\n", strerror(errno));
    }
}

