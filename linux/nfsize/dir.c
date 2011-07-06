/*
 * dir.c
 */

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "nfsize.h"

void is_dir(nfselem **nfs)
{
    struct stat statbuf;

    memset(&statbuf, 0, sizeof(struct stat));
    if (!stat((*nfs)->name, &statbuf)) {
        if (S_ISDIR(statbuf.st_mode)) {
            (*nfs)->d_def = true;
        }
    } else {
        die("is_dir(): %s\n", strerror(errno));
    }
}

void dir_contents(nfselem **nfs)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;

    if (!(*nfs)->d_def) {
        return;
    }
    (*nfs)->d_items = 0;
    if ((dir = opendir((*nfs)->name)) == NULL) {
        die("dir_contents(): opendir()");
    }
    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name,".") || !strcmp(entry->d_name,"..")) {
            continue;
        }
        (*nfs)->d_items++;
    }
    if (closedir(dir) == -1) {
        die("dir_contents(): closedir()");
    }
}

static bool d_is_file(char *p)
{
    FILE *file;

    if ((file = fopen(p,"r")) == NULL) {
        return false;
    }
    fclose(file);
    return true;
}

static bool d_is_dir(char *p)
{
    char *cwd = get_current_dir_name();

    if (cwd == NULL) {
        die("d_is_dir(): get_current_dir_name()");
    }
    if (chdir(p) != 0) {
        return false;
    }
    (void)chdir(cwd);
    free(cwd);
    return true;
}

/*static int private_d_getbytes(char *p)
{}*/

void d_getbytes(nfselem **nfs)
{
    /*DIR *dir = NULL;
    struct dirent *entry = NULL;

    if (!*(nfs)->d_def) {
        return;
    }
    if ((dir = opendir(*(nfs)->name)) == NULL) {
        die("d_getbytes(): opendir()");
    }
    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name,".") || !strcmp(entry->d_name,"..")) {
            continue;
        }
        char p[FILENAME_MAX];
    }*/
    return;
}

