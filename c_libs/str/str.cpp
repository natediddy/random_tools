/*
 * Simple custom string class
 * Nathan Forbes
 */

#include "str.h"

str::str(const char *s) {
   m_size = m_size_of(s);
   m_str = m_alloc(s, m_size);
}

str::~str() {
   delete[] m_str;
}

unsigned int str::m_size_of(const char *s) {
   const char *ss;
   unsigned int i;
   
   ss = s;
   
   for(i = 0; *ss != '\0'; *ss++, i++) {
   }
   
   return(i);
}

char *str::m_alloc(const char *s, unsigned int i) {
   unsigned int j, t;
   char *new_s;
   const char *ss;
   
   t = i + 1;
   new_s = new char[t];
   ss = s;
   
   for(j = 0; *ss != '\0'; *ss++, j++) {
      new_s[j] = *ss;
   }
   
   if(new_s[t] != '\0') {
      new_s[t] = '\0';
   }
   
   return(new_s);
}

unsigned int str::size() {
   return(m_size);
}

char *str::get() {
   return(m_str);
}

bool str::endswith(const char *p) {
   unsigned int j, p_size;
   bool e;
   int i;
   
   j = 0;
   p_size = m_size_of(p);
   e = false;
   
   for(i = (m_size - p_size); i < m_size; i++, j++) {
      if(m_str[i] != p[j]) {
         if(e) e = false;
         break;
      }
      if(!e) e = true;
   }
   
   return(e);
}

bool str::startswith(const char *p) {
   unsigned int i, p_size;
   bool s;
   
   p_size = m_size_of(p);
   s = false;
   
   for(i = 0; i < p_size; i++) {
      if(m_str[i] != p[i]) {
         if(s) s = false;
         break;
      }
      if(!s) s = true;
   }
   
   return(s);
}

void str::cat(const char *s) {
   unsigned int i, t;
   char *new_s;
   const char *s1;
   const char *s2;
   
   i = 0;
   t = m_size + m_size_of(s) + 1;
   new_s = new char[t];
   s1 = m_str;
   s2 = s;
   
   for(; *s1 != '\0'; *s1++, i++) {
      new_s[i] = *s1;
   }
   
   for(; *s2 != '\0'; *s2++, i++) {
      new_s[i] = *s2;
   }
   
   if(new_s[t] != '\0') {
      new_s[t] = '\0';
   }
   
   m_size = m_size_of(new_s);
   m_str = new_s;
}

bool str::comp(const char *s) {
   const char *s1;
   const char *s2;
   bool c;
   
   s1 = m_str;
   s2 = s;
   c = false;
   
   for(; *s1 != '\0' || *s2 != '\0'; *s1++, *s2++) {
      if(*s1 != *s2) {
         if(c) c = false;
         break;
      }
      if(!c) c = true;
   }
   
   return(c);
}

