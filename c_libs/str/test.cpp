/*
 * Series of tests for str class
 */

#include <iostream>
#include "str.h"

using std::cout;
using std::endl;

int main(int argc, char **argv) {
   str test1("test string number 1");
   str *test2 = new str("test string number 2");

   /* get() */
   cout << test1.get() << endl;
   cout << test2->get() << endl;

   /* size() */
   cout << "Size of test1 is " << test1.size() << endl;
   cout << "Size of test2 is " << test2->size() << endl;

   /* endswith() */
   if(test1.endswith("number 1")) {
      cout << "test1 endswith() pass" << endl;
   } else {
      cout << "test1 endswith() fail" << endl;
   }

   if(test2->endswith("er 2")) {
      cout << "test2 endswith() pass" << endl;
   } else {
      cout << "test2 endswith() fail" << endl;
   }

   /* startswith() */
   if(test1.startswith("test ")) {
      cout << "test1 startswith() pass" << endl;
   } else {
      cout << "test1 startswith() fail" << endl;
   }

   if(test2->startswith("test str")) {
      cout << "test2 startswith() pass" << endl;
   } else {
      cout << "test2 startswith() fail" << endl;
   }

   /* comp() */
   if(!test1.comp(test2->get())) {
      cout << "test1 != test2, pass" << endl;
   } else {
      cout << "test1 == test2, fail" << endl;
   }

   /* cat() */
   test1.cat(" add some more");
   test2->cat(test1.get());
   cout << test1.get() << endl;
   cout << test2->get() << endl;
   cout << "Size of test1 is " << test1.size() << endl;
   cout << "Size of test2 is " << test2->size() << endl;

   delete test2;
   return(0);
}

