#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *prg;

static int
usage()
{
   printf("usage: %s [data1] [data2] [dataN] ...\n", prg);
   printf("All data is compared for inequality against [data1]\n");
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
   if(argc <= 2) {
      return usage();
   }
   if(argc > 2) {
      do {
         if(*argv == NULL) {
            break;
         }
         if(first) {
            if(strcmp(first_arg,*argv) == 0) {
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

