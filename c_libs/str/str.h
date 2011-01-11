/*
 * Simple custom string class
 * Nathan Forbes
 */

#ifndef STRING_H
#define STRING_H

class str {
   char *m_str;
   unsigned int m_size;
   unsigned int m_size_of(const char *);
   char *m_alloc(const char *, unsigned int);
public:
   str(const char *);
   ~str();
   unsigned int size();
   char *get();
   void cat(const char *);
   bool endswith(const char *);
   bool startswith(const char *);
   bool comp(const char *);
};

#endif /* STRING_H */

