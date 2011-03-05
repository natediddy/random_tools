#include <stdio.h>
#include <stdlib.h>

static const char *prg;

static int
usage()
{
  fprintf(stderr, "usage: %s [filename]\n", prg);
  /* TODO bug reporting */
  return EXIT_FAILURE;
}

static int
does_file_exist(file)
  FILE *file;
{
  if (file == NULL)
    return 0;
  fclose(file);
  return 1;
}

int
main(argc, argv)
  int    argc;
  char **argv;
{
  prg = *argv;
  if (!does_file_exist(fopen(argv[1],"r")))
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

