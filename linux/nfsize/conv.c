/*
 * conv.c
 */

#include "nfsize.h"

void convert(nfselem **nfs)
{
    if (!(*nfs)->bytes) {
        return;
    }
    const int KB = 1024;
    const int MB = 1024*1024;
    const int GB = 1024*(1024*1024);
    int final;
    char str[108], *abbr;

    if (((*nfs)->bytes/GB) > 0) {
        final = (*nfs)->bytes/GB;
        abbr = "GB";
    } else if (((*nfs)->bytes/MB) > 0) {
        final = (*nfs)->bytes/MB;
        abbr = "MB";
    } else if (((*nfs)->bytes/KB) > 0) {
        final = (*nfs)->bytes/KB;
        abbr = "KB";
    } else {
        final = (*nfs)->bytes;
        abbr = "Bytes";
    }

    snprintf(str, 108, "%ld %s", final, abbr);
    (*nfs)->sz_str = strdup(str);
}

