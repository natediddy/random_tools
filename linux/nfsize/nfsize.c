/*
 * nfsize.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "nfsize.h"

static void nfsize_help(void)
{
    printf("Usage: %s [-h|--help] [FILE/DIR] ...\n"
            "Multiple files and/or directories can be provided at once\n",
            prog);
    exit(EXIT_SUCCESS);
}

static nfselem *new_nfselem(void)
{
    nfselem *nfs = (nfselem*)malloc(sizeof(nfselem));

    nfs->name = NULL;
    nfs->sz_str = NULL;
    nfs->f_def = false;
    nfs->d_def = false;
    nfs->bytes = 0;
    nfs->d_items = -1;
    return nfs;
}

static void parse_args(char ***argv, nfselem **nfs)
{
    *++(*argv);
    do {
        if (**argv == NULL) {
            break;
        }
        nfselem *tmp;
        if ((*nfs) == NULL) {
            tmp = new_nfselem();
            tmp->prev = NULL;
        } else {
            for (tmp = (*nfs); tmp != NULL; tmp = tmp->next) {
                if (tmp->next == NULL) {
                    break;
                }
            }
            tmp->next = new_nfselem();
            tmp->next->prev = tmp;
            tmp = tmp->next;
        }
        tmp->name = **argv;
        is_dir(&tmp);
        if (!tmp->d_def) {
            is_file(&tmp);
        }
        dir_contents(&tmp);
        d_getbytes(&tmp);
        f_getbytes(&tmp);
        convert(&tmp);
        tmp->next = NULL;
        if (tmp->prev != NULL) {
            for (; tmp != NULL; tmp = tmp->prev) {
                if (tmp->prev == NULL) {
                    break;
                }
            }
        }
        (*nfs) = tmp;
    } while (*(*argv)++);
}

static void display_results(nfselem **nfs)
{
    printf("--------------\n");
    for (; (*nfs) != NULL; (*nfs) = (*nfs)->next) {
        if ((*nfs)->prev != NULL) {
            if ((*nfs)->prev->sz_str != NULL) {
                free((*nfs)->prev->sz_str);
            }
            free((*nfs)->prev);
        }
        if ((*nfs)->f_def) {
            printf("FILE: ");
        } else if ((*nfs)->d_def) {
            printf("DIR: ");
        } else {
            printf("DOES NOT EXIST: ");
        }
        if ((*nfs)->name != NULL) {
            printf("%s\n", (*nfs)->name);
        }
        if ((*nfs)->d_def) {
            printf("ITEMS CONTAINED: %d\n", (*nfs)->d_items);
        }
        if ((*nfs)->sz_str != NULL) {
            printf("SIZE: %s\n", (*nfs)->sz_str);
        }
        printf("--------------\n");
        if ((*nfs)->next == NULL) {
            break;
        }
    }
    if ((*nfs) != NULL) {
        if ((*nfs)->sz_str != NULL) {
            free((*nfs)->sz_str);
        }
        free((*nfs));
    }
}

int main(int argc, char **argv)
{
    nfselem *nfselem = NULL;

    prog = *argv;
    if (argc > 1 && strstr(argv[1],"help") != NULL) {
        nfsize_help();
    } else {
        parse_args(&argv, &nfselem);
    }
    if (nfselem != NULL) {
        display_results(&nfselem);
    }
    return EXIT_SUCCESS;
}

