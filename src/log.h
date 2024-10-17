/*********************************
 * author     : mostafa khaled
 * date       : Wed Oct 16 03:07:22 PM EEST 2024
 * desc       : 
 ********************************/
#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#ifdef PRINT_LOG
  #define ASSERT(X) \
    if(!(X))        \
      fprintf(stderr, "\033[31mERROR(%s:%d): %s\033[0m\n", __FILE__, __LINE__, #X);

  #define LOG(X)    \
     fprintf(stderr, "\033[31mERROR(%s:%d): %s\033[0m\n", __FILE__, __LINE__, #X);
#else
  #define LOG(X)
  #define ASSERT(X)
#endif // PRINT_LOG

#endif /* !LOG_H */
