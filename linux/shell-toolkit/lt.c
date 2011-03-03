#include <stdio.h>
#include <stdlib.h>

static const char *prg;

static int
usage()
{
   printf("usage: %s [data1] [data2]\n", prg);
   printf("Only numerical values permitted\n");
   return EXIT_FAILURE;
}

int
main(argc, argv)
   int    argc;
   char **argv;
{
   int ret;
   float first, sec;

   prg = *argv;
   if(argc != 3) {
      return usage();
   }
   first = (float)atoi(argv[1]);
   sec = (float)atoi(argv[2]);
   if(first < sec) {
      ret = EXIT_SUCCESS;
   } else {
      ret = EXIT_FAILURE;
   }
   return ret;
}

