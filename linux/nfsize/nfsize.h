/*
 * nfsize.h
 */

#ifndef NFSIZE_H
#define NFSIZE_H

typedef struct nfsize_element nfselem;
typedef unsigned char bool;
#define false (bool)0
#define true  (bool)1

struct nfsize_element
{
    char *name, *sz_str;
    unsigned int bytes;
    bool f_def, d_def;
    int d_items;
    struct nfsize_element *next, *prev;
};

const char *prog;

void die(const char*,...);
void is_file(nfselem**);
void is_dir(nfselem**);
void dir_contents(nfselem**);
void convert(nfselem**);
void f_getbytes(nfselem**);
void d_getbytes(nfselem**);

#endif /* !NFSIZE_H */

