#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *prg;

static int
usage()
{
   fprintf(stderr, "usage: %s [data1] [data2] [dataN] ...\n", prg);
   fprintf(stderr, "All data is compared for equality against [data1]\n\n");
   fprintf(stderr, "Report bugs/issues to 'me AT nathanforbes DOT com'\n");
   return EXIT_FAILURE;
}

int
main(argc, argv)
   int    argc;
   char **argv;
{
   int ret, first = 0, err = 0;
   char *first_arg;

   prg = *argv;
   *++argv;
   if(argc == 1 || argc == 2) {
      return usage(1);
   }
   if(argc > 1) {
      do {
         if(*argv == NULL) {
            break;
         }
         if(first) {
            if(strcmp(first_arg,*argv) != 0) {
               err++;
            }
         }
         if(!first) {
            first_arg = *argv;
            first++;
         }
      } while(*argv++);
   }
   if(err != 0) {
      return EXIT_FAILURE;
   }
   return EXIT_SUCCESS;
}

