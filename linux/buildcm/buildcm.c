/*
 * Copyright (C) 2011 Nathan Forbes
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following disclaimer
 *       in the documentation and/or other materials provided with the
 *       distribution.
 *     * Neither the name of Nathan Forbes nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define DEFAULT_PROG_NAME "buildcm"
#define DEFAULT_SRC_DIR   "Android/src/cyanogenmod"
#define DEFAULT_TARGET    "sholes"

static char *prog;
static char *userhome;
static char *srcdir = NULL;
static char *target = NULL;
static unsigned int do_reposync = 0;
static unsigned int do_makeinstallclean = 0;

static void die(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "%s: error: ", prog);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    exit(EXIT_FAILURE);
}

enum filetype {
    REGFILE, DIRECTORY, EXEFILE
};

static unsigned int _fs_elem_exists(enum filetype type, const char *s)
{
    if (type == EXEFILE)
        return !access(s, X_OK);
    struct stat statbuf;
    memset(&statbuf, 0, sizeof(struct stat));
    if (!stat(s, &statbuf)) {
        if (type == REGFILE) {
            if (S_ISREG(statbuf.st_mode))
                return 1;
        }
        if (type == DIRECTORY) {
            if (S_ISDIR(statbuf.st_mode))
                return 1;
        }
    }
    return 0;
}

static unsigned int file_exists(const char *f)
{
    return _fs_elem_exists(REGFILE, f);
}

static unsigned int dir_exists(const char *d)
{
    return _fs_elem_exists(DIRECTORY, d);
}

static unsigned int is_executable(const char *e)
{
    return _fs_elem_exists(EXEFILE, e);
}

static char *scriptname()
{
    const char *path = "/tmp/buildcm-auto";
    static char buf[25];

    snprintf(buf, 25, "%s-%d", path, getpid());
    return buf;
}

static void writescript(FILE **stream)
{
    int x;
    unsigned int sync_cmd = 0, clean_cmd = 0;
    char *content[] = {
        "#!/bin/bash\n", "me=", prog, "\n",
        "sched() { schedtool -B -n 1 -e ionice -n 1 \"$@\"; }\n",
        "timediff() { local start=$1; local end=$2;"
            "total=$(expr $end - $start); min=$(expr $total / 60);"
            "sec=$(expr $total % 60); }\n",
        "src=", srcdir, ";", "START=$(date +%s);", "pushd $src;sleep 2;",
        ". build/envsetup.sh;lunch cyanogen_", target, "-eng;\n",
        "# PLACE HOLDER\n", "# PLACE HOLDER\n", "sched make -j4 bacon;",
        "END=$(date +%s);", "fn=$(ls $OUT | grep .zip$ | grep -v cy);",
        "timediff $START $END;",
        "echo \"$me: time: ${min} minutes ${sec} seconds\";",
        "[ -z $fn ] && { echo \"$me: build failed\"; exit 1; }\n",
        "mkdir -p ~/CMBuilds;",
        "ver=$(echo $fn | tr '-' ' ' | awk {'print $3-\"-\"$4'});",
        "ffn=cm-homegrown_$ver-$(date +%F)_$(date +%I%M%P)-signed.zip;",
        "mv $OUT/$fn ~/CMBuilds/$ffn;",
        "if [ $(adb devices | wc -l) -eq 3 ];then "
            "echo \"$me: device detected\";"
            "echo \"$me: pushing \\`$ffn' to device SD card\";"
            "adb push ~/CMBuilds/$ffn /sdcard/$ffn;"
            "(( $? )) && echo \"$me: ADB push failed\""
            "||echo \"$me: ADB push succeeded\"; fi;popd;exit 0;\n",
        NULL
    };

    for (x = 0; content[x] != NULL; x++) {
        if (x > 0  && (((do_reposync && !sync_cmd) ||
            (do_makeinstallclean && !clean_cmd)) && content[x][0] == '#')) {
            if (do_reposync && !sync_cmd) {
                char buf[FILENAME_MAX];
                snprintf(buf, FILENAME_MAX, "%s/bin/repo sync -j 10;\n",
                    userhome);
                content[x] = buf;
                sync_cmd++;
            } else if (do_makeinstallclean && !clean_cmd) {
                content[x] = "sched make -j 4 installclean;\n";
                clean_cmd++;
            }
        }
        fputs(content[x], (*stream));
    }
}

static char *touchscript()
{
    FILE *script = NULL;
    char *name = scriptname();
    char *errmsg = NULL;

    if (name && *name) {
        script = fopen(name, "w");
        if (script != NULL) {
            writescript(&script);
            fclose(script);
        } else {
            errmsg = "could not create script file";
            goto fail;
        }
    } else {
        errmsg = "could not generate script name";
        goto fail;
    }

    if (file_exists(name)) {
        if (chmod(name, 0755) == -1) {
            errmsg = strerror(errno);
            goto fail;
        } else if (!is_executable(name)) {
            errmsg = strerror(errno);
            goto fail;
        }
    }
    return name;
fail:
    if (script != NULL)
        fclose(script);
    die("%s\n", errmsg);
    return (char *)NULL;
}

static void getargs(char ***av)
{
    (void)*++(*av);
    do {
        if (**av == NULL)
            break;
        if (!strcmp(**av, "sync")) {
            if (!do_reposync)
                do_reposync++;
        } else if (!strcmp(**av, "clean")) {
            if (!do_makeinstallclean)
                do_makeinstallclean++;
        } else {
            if (dir_exists(**av)) {
                if (srcdir == NULL)
                    srcdir = **av;
            } else if (target == NULL)
                target = **av;
            else
                fprintf(stderr, "%s: `%s' unrecognized\n", prog, **av);
        }
    } while (*(*av)++);
    
    if (srcdir == NULL) {
        char buf[FILENAME_MAX];
        snprintf(buf, FILENAME_MAX, "%s/%s", userhome, DEFAULT_SRC_DIR);
        if (buf != NULL)
            srcdir = buf;
        else
            die("could not handle source directory string\n");
    }

    if (target == NULL)
        target = DEFAULT_TARGET;
}

static void buildcm()
{
    char *filename = touchscript();
    char *argv[] = { NULL };
    char *envp[] = { NULL };

    if (filename && *filename) {
        execve(filename, argv, envp);
        perror("execve");
    }
}

static void set_prog_name(char *p)
{
    char *pp = strrchr(p, '/');

    if (pp != NULL) {
        (void)*++pp;
        prog = pp;
    } else
        prog = DEFAULT_PROG_NAME;
}

int main(int ac, char **av)
{
    set_prog_name(*av);
    userhome = getenv("HOME");

    if (userhome && *userhome) {
        getargs(&av);
        buildcm();
    } else
        fprintf(stderr, "%s: could not get user home directory\n", prog);
    return EXIT_FAILURE;
}

