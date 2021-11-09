#include <stdio.h>
#include <stddef.h>
#include <inttypes.h>

#include "my_struct.h"
#include "my_struct_utils.h"


#define BL_I(s) blob_init(sizeof(s)-1, (uint8_t [sizeof(s)-1]){s})

#define BL_N(s) node_elm_init_b(BL_I(s))

#define ST_I(...) \
  stack_init \
  ( \
  sizeof((node_elm []){__VA_ARGS__})/sizeof(node_elm), \
  (node_elm []){__VA_ARGS__} \
  )

#define ST_N(...) node_elm_init_st(ST_I(__VA_ARGS__))


int main(int argc, char *argv[], char *envp[])
{
  stack *a = stack_scan_fromstream2(stdin);
  stack_print(a,0,2);
  stack_free(a);
  #if 0
  // testing stuff
  if (0)
  {


  blob *bl = blob_scan_fromstream(stdin);
  //printf("test");
  //printf("%zu\n", bl->len);
  blob_print(bl);
  stack *a =
    ST_I(
      BL_N("a\n\nbc\"0\""),
      BL_N("abc1"),
      BL_N("abc2"),
      BL_N("abc3"),
      BL_N("abc4"),
      ST_N
      (
        BL_N
        ("!abc0"),
        BL_N("!abc1\x00"),
        BL_N("!abc2"),
        BL_N("!abc3"),
        BL_N("!abc4"),
        ST_N
        (
          BL_N("!!abc0"),
          BL_N("!!abc1"),
          BL_N("!!abc2"),
        )
      ),
      BL_N("abc5")
    );
  /*
   stack *a = stack_init
   (
     4,
     (node_elm[4])
     {
       NODE_S("abc"),
       //node_elm_init_b(blob_init("123a", 4)),
       node_elm_init_b(blob_init("234b", 4)),
       node_elm_init_b(blob_init("345c", 4)),

       node_elm_init_st(stack_init_list
       (
         3,
         node_elm_init_b(blob_init("lalala1", 7)),
         node_elm_init_b(blob_init("lalala2", 7)),
         node_elm_init_b(blob_init("lalala3", 7))
       ))
     }
   );*/
  stack_print(a,0,2);
  printf("is same a a: %d\n", stack_cmp(a,a));


    stack *b =
    ST_I(
      BL_N("a\n\nbc\"0\""),
      BL_N("abc1"),
      BL_N("abc2"),
      BL_N("abc3"),
      BL_N("abc4"),
      ST_N
      (
        BL_N
        ("!abc0"),
        BL_N("!abc1\x00"),
        BL_N("!abc2"),
        BL_N("!abc3"),
        BL_N("!abc4"),
        ST_N
        (
          BL_N("!!abc0"),
          BL_N("!!abc1"),
          BL_N("!!abc2"),
        )
      ),
      BL_N("abc5")
    );


  printf("is same a b: %d\n", stack_cmp(a,b));

  stack_free(a);
  }
  #endif
  return 0;
}
