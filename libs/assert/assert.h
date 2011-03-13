/*
 * Simple "assert" function for C++
 * Nathan Forbes
 */

#ifndef ASSERT_H
#define ASSERT_H

#include <iostream>
#include <sstream>

#define SHOW_ASSERT_FAILUIRE(msg) {                         \
   std::ostringstream s;                                    \
   s << "Internal error in " << __FILE__ << ':'             \
   << __LINE__ << ": \n\t" << msg << '\0';                  \
   present_error("Internal Error", s.str().c_str());        \
}

#define ASSERT(x) {                                \
   if(!x) {                                        \
      const char *msg = "Assertion " #x " failed"; \
      SHOW_ASSERT_FAILUIRE(msg);                   \
   }                                               \
}

static inline void present_error(const char *caption, const char *message)
{
   std::cerr << caption << ": " << message << std::endl;
}

#endif /* ASSERT_H */

