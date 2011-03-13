/*
 * String operations
 */

#include "str.h"

int s_size_of(const char *s) {
    int size;
    const char *ss;

    ss = s;

    for (size = 0; *ss != '\0'; *ss++, size++) {
    }

    return (size);
}

/*
 * s = string
 * p = pattern
 * 1 = true
 * 0 = false
 */
int s_endswith(const char *s, const char *p) {
    int s_size, p_size, i, j, endswith;

    s_size = s_size_of(s);
    p_size = s_size_of(p);
    j = 0;
    endswith = 0;

    if (s_size < p_size) {
        return (0);
    }

    for (i = (s_size - p_size); i < s_size; i++, j++) {
        if (s[i] != p[j]) {
            if (endswith) {
                endswith = 0;
            }
            break;
        }
        if (!endswith) {
            endswith++;
        }
    }

    return (endswith);
}

/*
 * s = string
 * p = pattern
 * 1 = true
 * 0 = false
 */
int s_startswith(const char *s, const char *p) {
    int p_size, i, startswith;

    p_size = s_size_of(p);
    startswith = 0;

    for (i = 0; i < p_size; i++) {
        if (s[i] != p[i]) {
            if (startswith) {
                startswith = 0;
            }
            break;
        }
        if (!startswith) {
            startswith++;
        }
    }

    return (startswith);
}

int s_compare(const char *s1, const char *s2) {
    int i, s1s, eq;

    s1s = s_size_of(s1);
    eq = 0;

    if (s1s != s_size_of(s2)) {
        return (0);
    }

    for (i = 0; i < s1s; i++) {
        if (s1[i] != s2[i]) {
            if (eq) {
                eq = 0;
            }
            break;
        }
        if (!eq) {
            eq++;
        }
    }

    return (eq);
}

char *s_cat(const char *s1, const char *s2) {
    int i, j, total, s1s, s2s;
    char *ss;
    const char *ss1;
    const char *ss2;

    s1s = s_size_of(s1);
    s2s = s_size_of(s2);
    total = s1s + s2s + 1;
    ss1 = s1;
    ss2 = s2;

    char tmp[total];

    for (i = 0; *ss1 != '\0'; *ss1++, i++) {
        tmp[i] = *ss1;
    }

    for (j = i; *ss2 != '\0'; *ss2++, j++) {
        tmp[j] = *ss2;
    }

    if (tmp[total] != '\0') {
        tmp[total] = '\0';
    }

    ss = tmp;
    return (ss);
}

/*
 * s = string
 * p = pattern
 * 0 = false
 * 1 = true
 */
int s_contains(const char *s, const char *p) {
    int c, p_size;
    const char *ss;
    const char *pp;

    ss = s;
    pp = p;
    p_size = s_size_of(p);
    c = 0;

    for (;; *ss++) {
        if (*ss == '\0' || *pp == '\0') {
            break;
        }
        if (*ss == *pp) {
            *pp++;
            c++;
        }
    }

    if (c != p_size) {
        return (0);
    }

    return (1);
}
