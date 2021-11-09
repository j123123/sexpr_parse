#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "alloc_routines.h"
#define DBG
#ifdef DBG

#define PRV_ERR_MACRO() \
  do{\
    fprintf\
    ( \
      stderr, \
      "[error] at file \"%s\", func \"%s\", line %d\n", \
      __FILE__, __func__, __LINE__ \
    ); \
    exit(EXIT_FAILURE); \
  } while(0);

#else
#define PRV_ERR_MACRO() exit(EXIT_FAILURE);
#endif

#define STACK_T 0
#define BLOB_T 1

typedef size_t stack_sz;
typedef size_t blob_sz;


typedef struct node_elm node_elm;
typedef struct stack stack;
typedef struct blob blob;
typedef struct elm_t elm_t;

struct elm_t
{
  size_t t;
};

struct blob
{
  blob_sz len;
  uint8_t data[];
};


struct node_elm
{
  elm_t data_t;
  union
  {
    stack *st;
    blob *b;
  } u;
};

struct stack
{
  stack_sz len;
  size_t sz_bytes;
  node_elm nodes[];
};


// function definitions

blob_sz
blob_getlen
(
  blob *in
);


uint8_t *
blob_getdata
(
  blob *in
);


blob *
blob_init
(
  blob_sz len,
  uint8_t *data
);


bool
blob_cmp
(
  blob *a,
  blob *b
);


void
blob_free
(
  blob *in
);


stack *
stack_init_list
(
  stack_sz len,
  ...
);


stack *
stack_init
(
  stack_sz len,
  node_elm arr[len]
);


void
stack_free
(
  stack *st
);


void
node_elm_free
(
  node_elm *in
);


bool
node_elm_cmp
(
  node_elm *a,
  node_elm *b
);

node_elm
node_elm_init_b
(
  blob *in
);

node_elm
node_elm_init_st
(
  stack *in
);


bool
stack_cmp
(
  stack *a,
  stack *b
);


void
stack_push
(
  stack **st,
  node_elm node
);
