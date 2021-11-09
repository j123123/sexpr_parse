#pragma once

#include <stdio.h>
#include <stdlib.h>

#define PRV_MALLOC(x) xmalloc(x)
#define PRV_REALLOC(a,b) xrealloc(a,b)
#define PRV_FREE(x) my_free(x)

/*
#define PRV_FREE(x) \
  do{ \
    my_free(x), \
    fprintf\
    ( \
      stderr, \
      "[dbg] at file \"%s\", func \"%s\", line %d\n", \
      __FILE__, __func__, __LINE__ \
    ); \
  }while(0)
*/

void *
xmalloc
(
  size_t size
);

void *
xrealloc
(
  void *ptr,
  size_t size
);

void
my_free
(
  void *ptr
);
