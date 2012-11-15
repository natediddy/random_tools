/*
 * copy.c
 *
 * Copy a file from one location to another with a progress bar.
 * Doesn't work on copying directories at the moment, and
 * is mainly for educational purposes creating the progress bar.
 * It seems to work fine but is EXTREMELY slow when compared
 * to something like `cp'.
 *
 * Nathan Forbes
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define DEFAULT_PROGRESSBAR_WIDTH 40

#define KB_FACTOR 1024LU
#define MB_FACTOR (KB_FACTOR * KB_FACTOR)
#define GB_FACTOR (KB_FACTOR * MB_FACTOR)
#define TB_FACTOR (KB_FACTOR * GB_FACTOR)

typedef unsigned char bool;

#define false ((bool)0)
#define true ((bool)1)

const char *program_name;

static void print_usage(bool error)
{
    fprintf(!error ? stdout : stderr, "usage: %s SOURCE DESTINATION\n",
            program_name);
}

static char *bytes_readable(char *buffer, size_t bytes)
{
    if (!bytes) {
        strncpy(buffer, "0B", 2);
        return;
    }

    char abbr;
    double final;

    if ((bytes / TB_FACTOR) > 0) {
        final = (double)bytes / (double)TB_FACTOR;
        abbr = 'T';
    } else if ((bytes / GB_FACTOR) > 0) {
        final = (double)bytes / (double)GB_FACTOR;
        abbr = 'G';
    } else if ((bytes / MB_FACTOR) > 0) {
        final = (double)bytes / (double)MB_FACTOR;
        abbr = 'M';
    } else if ((bytes / KB_FACTOR) > 0) {
        final = (double)bytes / (double)KB_FACTOR;
        abbr = 'K';
    } else {
        final = (double)bytes;
        abbr = 'B';
    }
    snprintf(buffer, sizeof(buffer), "%.1f%c", final, abbr);
}

static char *make_absolute(const char *relp)
{
    if (*relp == '/') {
        return strdup(relp);
    }

    char curdir[FILENAME_MAX];

    if (getcwd(curdir, FILENAME_MAX) != NULL) {
        size_t size = strlen(curdir) + strlen(relp) + 2;
        char *buf = (char *)malloc(size);
        if (buf != NULL) {
            snprintf(buf, size, "%s/%s", curdir, relp);
            return buf;
        } else {
            fprintf(stderr, "%s: error: failed to allocate %lu bytes\n",
                    program_name, (unsigned long)size);
        }
    } else {
        fprintf(stderr, "%s: error: could not get current directory\n",
                program_name);
    }
    return NULL;
}

static char *get_basename(const char *path)
{
    char *p = (char *)path;

    while (p[strlen(p) - 1] == '/') {
        p[strlen(p) - 1] = '\0';
    }

    char *s = strrchr(p, '/');
    return ++s;
}

static int round_frac(double frac)
{
    int result;

    if (frac >= 0) {
        result = (int)(frac + 0.5);
    } else {
        result = (int)(frac - 0.5);
    }
    return result;
}

static bool isNaN(double v)
{
    volatile double c = v;

    if (c != c) {
        return true;
    }
    return false;
}

static char *percentstr(char *buffer, double v)
{
    if (isNaN(v) || isNaN(v * 100)) {
        strncpy(buffer, "0%", 2);
        return;
    }
    snprintf(buffer, sizeof(buffer), "%3.0f%%", v * 100);
}

static int console_width(void)
{
    struct winsize ws;
    int width = 0;

    memset(&ws, 0, sizeof(struct winsize));
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    width = ws.ws_col;
    if (width <= 0) {
        width = DEFAULT_PROGRESSBAR_WIDTH;
    }
    return width;
}

struct progress_bar {
    int width;
    int space_til_end;
    size_t pos;
    char buffer[256];
} *pbar = NULL;

static void pbar_init(void)
{
    pbar = (struct progress_bar *)malloc(sizeof(struct progress_bar));
    pbar->width = console_width();
    pbar->pos = 0;
    pbar->space_til_end = pbar->width;
}

static void pbar_add(const char *s, bool space, bool last)
{
    if (last) {
        int i;
        for (i = 0; i < (pbar->space_til_end - 4); ++i, ++pbar->pos) {
            pbar->buffer[pbar->pos] = ' ';
        }
    }

    size_t len = strlen(s);

    if (len > 1) {
        strncpy(pbar->buffer + pbar->pos, s, len);
        pbar->pos += len;
        pbar->space_til_end -= len;
    } else if (len == 1) {
        pbar->buffer[pbar->pos] = s[0];
        ++pbar->pos;
        --pbar->space_til_end;
    }

    if (space) {
        pbar->buffer[pbar->pos] = ' ';
        ++pbar->pos;
        --pbar->space_til_end;
    }
}

static void pbar_display(void)
{
    if (*pbar->buffer == '\0') {
        return;
    }

    pbar->buffer[pbar->pos] = '\r';
    pbar->buffer[pbar->pos + 1] = '\0';
    fputs(pbar->buffer, stdout);
    fflush(stdout);
    *pbar->buffer = '\0';
    pbar->pos = 0;
    pbar->space_til_end = pbar->width;
}

static bool do_copy_operation(const char *sp,
                              const char *dp,
                              size_t size,
                              mode_t mode)
{
    FILE *s = NULL;
    FILE *d = NULL;

    s = fopen(sp, "rb");
    if (s == NULL) {
        fprintf(stderr, "%s: error: failed to open `%s' for reading\n",
                program_name, sp);
        return false;
    }

    d = fopen(dp, "w+b");
    if (d == NULL) {
        fprintf(stderr, "%s: error: failed to open `%s' for writing\n",
                program_name, dp);
        if (s != NULL) {
            fclose(s);
        }
        return false;
    }

    int byte;
    size_t cp_so_far = 0;
    time_t start_epoch = time(NULL);

    while ((byte = fgetc(s)) != EOF) {
        if (fputc(byte, d) == EOF) {
            fprintf(stderr, "%s: error: could not write data\n",
                    program_name);
            fclose(s);
            fclose(d);
            return false;
        }
        cp_so_far += 1;
        double frac_cp = (double)cp_so_far / (double)size;
        int bar_pos_stop;
        char cp_so_far_str[7];
        char tot_to_cp_str[7];
        char perc[5];
        int pos;
        int i;
        if (pbar) {
            pbar->width = console_width();
        } else {
            pbar_init();
        }
        bytes_readable(cp_so_far_str, cp_so_far);
        bytes_readable(tot_to_cp_str, size);
        pbar_add(cp_so_far_str, false, false);
        pbar_add("/", false, false);
        pbar_add(tot_to_cp_str, true, false);
        pbar_add("[", false, false);
        bar_pos_stop = pbar->width -
            (strlen(cp_so_far_str) + strlen(tot_to_cp_str) + 9);
        pos = round_frac(frac_cp * (double)bar_pos_stop);
        for (i = 0; i < pos; ++i) {
            pbar_add("#", false, false);
        }
        for (; i < bar_pos_stop; ++i) {
            pbar_add(" ", false, false);
        }
        pbar_add("]", true, false);
        percentstr(perc, frac_cp);
        pbar_add(perc, false, true);
        pbar_display();
        if (cp_so_far >= size) {
            free(pbar);
            pbar = NULL;
        }
    }

    fputc('\n', stdout);
    fclose(s);
    fclose(d);

    if (chmod(dp, mode) == -1) {
        fprintf(stderr, "%s: error: `%s': %s\n",
                program_name, dp, strerror(errno));
        return false;
    }

    return true;
}

int main(int argc, char **argv)
{
    program_name = argv[0];
    if (argc != 3) {
        print_usage(true);
        return EXIT_FAILURE;
    }

    int ret = EXIT_SUCCESS;
    struct stat st;
    char *spath = make_absolute(argv[1]);
    char *dpath = make_absolute(argv[2]);
    bool src_is_dir = false;
    bool dst_is_dir = false;
    size_t src_size = 0;
    mode_t src_mode = 0;

    if (spath == NULL || dpath == NULL) {
        ret = EXIT_FAILURE;
        goto done;
    }

    memset(&st, 0, sizeof(struct stat));
    if (stat(spath, &st) == 0) {
        if (S_ISDIR(st.st_mode)) {
            src_is_dir = true;
        } else if (!S_ISREG(st.st_mode)) {
            fprintf(stderr, "%s: error: source file `%s' is neither a file "
                    "nor a directory\n", program_name, spath);
            ret = EXIT_FAILURE;
            goto done;
        } else {
            src_size = st.st_size;
            src_mode = st.st_mode;
        }
    } else {
        fprintf(stderr, "%s: error: source file `%s' does not exist\n",
                program_name, spath);
        ret = EXIT_FAILURE;
        goto done;
    }

    if (src_is_dir) {
        fprintf(stderr, "%s: error: use of directory as source is not "
                "supported at this time\n", program_name);
        ret = EXIT_FAILURE;
        goto done;
    }

    memset(&st, 0, sizeof(struct stat));
    if (stat(dpath, &st) == 0) {
        if (S_ISDIR(st.st_mode)) {
            dst_is_dir = true;
        } else {
            fprintf(stderr, "%s: error: destination `%s' already exists\n",
                    program_name, dpath);
            ret = EXIT_FAILURE;
            goto done;
        }
    }

    if (dst_is_dir) {
        char *spath_bn = get_basename(spath);
        size_t t_size = strlen(dpath) + strlen(spath_bn) + 2;
        char buf[t_size];
        snprintf(buf, t_size, "%s/%s", dpath, spath_bn);
        free(dpath);
        dpath = NULL;
        dpath = strdup(buf);
        if (dpath == NULL) {
            fprintf(stderr, "%s: error: failed to allocate %lu bytes\n",
                    program_name, (unsigned long)t_size);
            ret = EXIT_FAILURE;
            goto done;
        }
    }

    if (!do_copy_operation(spath, dpath, src_size, src_mode)) {
        ret = EXIT_FAILURE;
    }

done:
    if (spath != NULL) {
        free(spath);
        spath = NULL;
    }
    if (dpath != NULL) {
        free(dpath);
        dpath = NULL;
    }
    return ret;
}

