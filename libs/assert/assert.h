/*
 * Simple assert functionality for C++
 *
 * Used as:
 *    #include "assert.h"
 *    ...
 *    assert(condition);
 *
 * Nathan Forbes
 */

#ifndef INCLUDED_ASSERT_H
#define INCLUDED_ASSERT_H

#include <iostream>
#include <sstream>

#define SHOW_ASSERT_FAILUIRE(msg) {                         \
   std::ostringstream s;                                    \
   s << "in " << __FILE__ << ':'                            \
   << __LINE__ << ": \n\t" << msg << '\0';                  \
   __present_error("Assertion", s.str().c_str());           \
}

#define assert(condition) {                                 \
   if((condition)) {                                        \
      const char *msg = "Assertion " #condition " caught";  \
      SHOW_ASSERT_FAILUIRE(msg);                            \
   }                                                        \
}

static inline void __present_error(const char *c, const char *m) {
   std::cerr << c << ": " << m << std::endl;
}

#endif /* INCLUDED_ASSERT_H */

