/*
 * My implementation of the Unix `which' program.
 * Uses less memory and almost no CPU.
 *
 * Nathan Forbes
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static int
file_exists (const char *fn)
{
  struct stat stbuf;

  memset (&stbuf, 0, sizeof (struct stat));
  if (!stat (fn, &stbuf) && S_ISREG (stbuf.st_mode) && !access (fn, X_OK))
    return 1;
  return 0;
}

static char *
which (const char *prg)
{
  const char *path_list = getenv ("PATH");
  static char buffer[FILENAME_MAX];
  size_t pos, i;

  for (i = 0, pos = 0; path_list[pos]; pos++) {
    if (i > 0 && path_list[pos] == ':') {
      buffer[i] = '/';
      strcpy (buffer + (i + 1), prg);
      if (file_exists (buffer))
        return buffer;
      i = 0;
    } else
      buffer[i++] = path_list[pos];
  }
  return NULL;
}

int
main (int argc, char **argv)
{
  int retval = EXIT_SUCCESS;

  (void) *++argv;
  do {
    if (!*argv)
      break;
    char *p = which (*argv);
    if (p && *p)
      puts (p);
    else if (retval != EXIT_FAILURE)
      retval = EXIT_FAILURE;
  } while (*argv++);
  return retval;
}

