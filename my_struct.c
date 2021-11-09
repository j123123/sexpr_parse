#include <inttypes.h>
#include <stddef.h>
#include <limits.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include "alloc_routines.h"

#include "my_struct.h"


blob_sz
blob_getlen
(blob *in)
{
  return in->len;
}


uint8_t *
blob_getdata
(blob *in)
{
  return in->data;
}


blob *
blob_init
(
  blob_sz len,
  uint8_t *data
)
{
  blob *ret = PRV_MALLOC(sizeof(len)+len);
  ret->len = len;
  memcpy(ret->data, data, len);
  return ret;
}


bool
blob_cmp
(
  blob *a,
  blob *b
)
{
  if (a->len != b->len)
  {
    return false;
  }
  if (memcmp(a->data, b->data, a->len) != 0)
  {
    return false;
  }
  return true;
}


void
blob_free
(
  blob *in
)
{
  PRV_FREE(in);
}


void node_elm_free
(
  node_elm *in
)
{
  if(in->data_t.t == BLOB_T)
  {
    return blob_free(in->u.b);
  }
  if(in->data_t.t == STACK_T)
  {
    stack_free(in->u.st);
  }
  else
  {
    PRV_ERR_MACRO();
  }
}


bool node_elm_cmp
(
  node_elm *a,
  node_elm *b
)
{
  if(a->data_t.t != b->data_t.t)
  {
    return false;
  }
  if(a->data_t.t == BLOB_T)
  {
    return blob_cmp(a->u.b, b->u.b);
  }
  if(a->data_t.t == STACK_T)
  {
    return stack_cmp(a->u.st, b->u.st);
  }
  else
  {
    PRV_ERR_MACRO();
  }
}


node_elm
node_elm_init_b
(
  blob *in
)
{
  node_elm ret;
  ret.data_t.t = BLOB_T;
  ret.u.b = in;
  return ret;
}


node_elm
node_elm_init_st
(
  stack *in
)
{
  node_elm ret;
  ret.data_t.t = STACK_T;
  ret.u.st = in;
  return ret;
}


bool stack_cmp
(
  stack *a,
  stack *b
)
{
  if(a->len != b->len)
  {
    return false;
  }
  const size_t len = a->len;
  for(size_t i = 0; i < len; ++i)
  {
    if(node_elm_cmp(&a->nodes[i], &b->nodes[i]) == false)
    {
      return false;
    }
  }
  return true;
}


void
stack_free
(
  stack *st
)
{
  const size_t len = st->len;
  for(size_t i = 0; i < len; ++i)
  {
    node_elm_free(&st->nodes[i]);
  }
  PRV_FREE(st);
}


stack *
stack_init
(
  stack_sz len,
  node_elm arr[len]
)
{
  const size_t bytes_allocated =
    1ULL <<
    (
      CHAR_BIT*sizeof(unsigned long long)-
      __builtin_clzll(sizeof(stack)+len*sizeof(node_elm))
    );
  stack *ret = PRV_MALLOC(bytes_allocated);
  ret->len = len;
  ret->sz_bytes = bytes_allocated;
  memcpy(ret->nodes, arr, len*sizeof(*arr));
  return ret;
}


stack *
stack_init_list
(
  stack_sz len,
  ...
)
{
  va_list n_list;
  va_start(n_list,len);
  const size_t bytes_allocated =
    1ULL <<
    (
      CHAR_BIT*sizeof(unsigned long long)-
      __builtin_clzll(sizeof(stack)+len*sizeof(node_elm))
    );
  stack *ret = PRV_MALLOC(bytes_allocated);
  ret->len = len;
  ret->sz_bytes = bytes_allocated;
  for (size_t i = 0; i < len; ++i)
  {
    ret->nodes[i] = va_arg(n_list, node_elm);
  }
  va_end(n_list);
  return ret;
}


void
stack_push
(
  stack **st,
  node_elm node
)
{
  if
  (
    (*st)->sz_bytes <
    sizeof(stack)+((*st)->len + 1)*sizeof(node_elm)
  )
  {
    const size_t bytes_allocated = ((*st)->sz_bytes << 1 ); // I'm sure that it's OK.
    (*st) = PRV_REALLOC(*st,bytes_allocated);
    (*st)->sz_bytes = bytes_allocated;
  }
  (*st)->nodes[(*st)->len] = node;
  (*st)->len++;
}
