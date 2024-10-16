/*********************************
 * author     : mostafa khaled
 * date       : Wed Oct 16 03:07:22 PM EEST 2024
 * desc       : 
 ********************************/
#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#define assert(X) \
  if(!(X))        \
    fprintf(stderr, "\033[31mERROR(%s:%d): %s\033[0m\n", __FILE__, __LINE__, #X);

#define log(X)    \
    fprintf(stderr, "\033[31mERROR(%s:%d): %s\033[0m\n", __FILE__, __LINE__, #X);

#endif /* !LOG_H */
