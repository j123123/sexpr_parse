#include <stdio.h>
#include <stdlib.h>

//#define ALLOC_DBG

void *
xmalloc
(
  size_t size
)
{
#ifdef ALLOC_DBG
  fprintf(stderr, "[dbg] xmalloc(%zu)\n", size);
#endif

  void *new_mem = (void *) malloc (size);
  if (new_mem == NULL)
  {
    fprintf (stderr, "fatal: memory exhausted (xmalloc of %zu bytes).\n", size);
    exit (-1);
  }
  return new_mem;
}

void *
xrealloc
(
  void *ptr,
  size_t size
)
{
#ifdef ALLOC_DBG
  fprintf(stderr, "[dbg] xrealloc(%p, %zu)\n", ptr, size);
#endif

  void *new_mem = (void *) realloc (ptr, size);
  if (new_mem == NULL)
  {
    fprintf (stderr, "fatal: memory exhausted (xrealloc of %zu bytes).\n", size);
    exit (-1);
  }
  return new_mem;
}

void
my_free
(
  void *ptr
)
{
#ifdef ALLOC_DBG
  fprintf(stderr, "[dbg] my_free(%p)\n", ptr);
#endif

#if 0
  if(ptr == NULL)
  {
    fprintf (stderr, "fatal: null ptr passed to free()\n");
    exit (-1);
  }
#endif

  free(ptr);
}
