#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define KB 1024
#define MB (1024*1024)
#define GB ((1024*1024)*1024)

static const char *prg;

static int
usage()
{
  fprintf(stderr, "usage: %s [filename]\n", prg);
  /* TODO bug reporting */
  return EXIT_FAILURE;
}

/* boolean */
static unsigned int
does_file_exist(file)
  FILE *file;
{
  if (file == NULL)
    return 0;
  fclose(file);
  return 1;
}

static int
determine(bytes)
  int bytes;
{
  if ((bytes / GB) > 0)
    return 1;
  else if ((bytes / MB) > 0)
    return 2;
  else if ((bytes / KB) > 0)
    return 3;
  else
    return 0;
}

static int
file_size(bytes)
  int   bytes;
{
  int b, s;

  b = determine(bytes);
  s = 0.0;

  switch (b)
    {
      case 1:
        s = bytes / GB;
        break;
      case 2:
        s = bytes / MB;
        break;
      case 3:
        s = bytes / KB;
        break;
      default:
        s = bytes;
        break;
    }
  return s;
}

static char *
file_size_abbr(bytes)
  int bytes;
{
  int b;
  char *ab;

  b = determine(bytes);
  ab = NULL;

  switch (b)
    {
      case 1:
        ab = "GB";
        break;
      case 2:
        ab = "MB";
        break;
      case 3:
        ab = "KB";
        break;
      default:
        ab = "BYTES";
        break;
    }
  return ab;
}

static void
print_size(fname)
  char *fname;
{
  struct stat s;
  int bytes;

  stat(fname, &s);
  bytes = s.st_size;
  printf("%s: %d %s\n",
    fname,
    file_size(bytes),
    file_size_abbr(bytes));
}

int
main(argc, argv)
  int    argc;
  char **argv;
{
  int i;

  prg = *argv;
  if (argc < 2)
    return usage();
  if (!does_file_exist(fopen(argv[1],"r")))
    return -1;
  for (i = 1; argv[i] != NULL; i++)
    print_size(argv[i]);
  return EXIT_SUCCESS;
}

