#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *prg;

static int
usage()
{
   fprintf(stderr, "usage: %s [data1] [data2] [dataN] ...\n", prg);
   fprintf(stderr, "All data is compared for inequality against [data1]\n");
   fprintf(stderr, "Report bugs/issues to 'me AT nathanforbes DOT com'\n");
   return EXIT_FAILURE;
}

int
main(argc, argv)
   int    argc;
   char **argv;
{
   int first = 0, ne = 0;
   char *first_arg;

   prg = *argv;
   *++argv;
   if(argc <= 2) {
      return usage();
   }
   if(argc > 2) {
      do {
         if(*argv == NULL) {
            break;
         }
         if(first) {
            if(strcmp(first_arg,*argv) != 0) {
               ne++;
            }
         }
         if(!first) {
            first_arg = *argv;
            first++;
         }
      } while(*argv++);
   }
   if(ne != 0) {
      return EXIT_SUCCESS;
   }
   return EXIT_FAILURE;
}

