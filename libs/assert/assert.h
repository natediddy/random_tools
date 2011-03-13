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
   s << "error in " << __FILE__ << ':'                      \
   << __LINE__ << ": \n\t" << msg << '\0';                  \
   __present_error("Internal Error", s.str().c_str());        \
}

#define assert(x) {                                \
   if(!x) {                                        \
      const char *msg = "Assertion " #x " failed"; \
      SHOW_ASSERT_FAILUIRE(msg);                   \
   }                                               \
}

static inline void __present_error(const char *c, const char *m) {
   std::cerr << c << ": " << m << std::endl;
}

#endif /* INCLUDED_ASSERT_H */

