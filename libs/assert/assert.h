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

#define TAG "ASSERT"

static inline void display_assert(const char *s) {
  std::cout << TAG << ": " << s << std::endl;
}

#define SHOW_ASSERT_FAILURE(msg) {                           \
  std::ostringstream s;                                      \
  s << __FILE__ << ':' << __LINE__ << ": " << msg << '\0';   \
  display_assert(s.str().c_str());                           \
}

#define assert(condition) {                                  \
  if ((condition)) {                                         \
    const char *msg = "Condition: '"#condition "' is true."; \
    SHOW_ASSERT_FAILURE(msg);                                \
  }                                                          \
}

#endif /* !INCLUDED_ASSERT_H */

