/*
 * conv.c
 */

#include <string.h>
#include "nfsize.h"

#define KB 1024L
#define MB (KB * KB)
#define GB (KB * MB)

void convert(nfselem **nfs)
{
    if (!(*nfs)->bytes) {
        return;
    }
    double final;
    char str[30], *abbr;

    if (((*nfs)->bytes/(long)GB) > 0) {
        final = (double)(*nfs)->bytes/(double)GB;
        abbr = "GB";
    } else if (((*nfs)->bytes/(long)MB) > 0) {
        final = (double)(*nfs)->bytes/(double)MB;
        abbr = "MB";
    } else if (((*nfs)->bytes/(long)KB) > 0) {
        final = (double)(*nfs)->bytes/(double)KB;
        abbr = "KB";
    } else {
        final = (double)(*nfs)->bytes;
        abbr = "Bytes";
    }

    sprintf(str, "%.1f %s", final, abbr);
    (*nfs)->sz_str = strdup(str);
}

