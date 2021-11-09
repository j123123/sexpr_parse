#include "my_struct.h"
#include "my_struct_utils.h"
#include <ctype.h>
#include <stdio.h>
#include <inttypes.h>


void
blob_print
(
  blob *in
)
{
  const size_t sz = in->len;
  putchar('"');
  for(size_t i = 0; i < sz; ++i)
  {
    if(isprint((int)in->data[i]))
    {
      switch(in->data[i])
      {
        case '\\':
          fputs("\\\\", stdout);
          break;
        case   '\t':
          fputs("\\t", stdout);
          break;
        case   '"':
          fputs("\\\"", stdout);
          break;
        default:
          fputc((char)in->data[i], stdout);
      }
    }
    else
    {
      switch(in->data[i])
      {
        case '\t':
          fputs("\\t", stdout);
          break;

        case '\n':
          fputs("\\n", stdout);
          break;

        default:
          printf("\\x%02" PRIX8, in->data[i]);
      }
    }
  }
  putchar('"');
  printf("\n");
}

void
stack_print
(
  stack *st,
  size_t depth,
  size_t d_l
)
{
  for(size_t i = 0; i < depth*d_l; ++i)
  {
    putchar(' ');
  }
  fputs("(\n", stdout);
  const size_t len = st->len;
  for (size_t i = 0; i < len; ++i)
  {
    if(st->nodes[i].data_t.t == BLOB_T)
    {
      for(size_t j = 0; j < (depth+1)*d_l; ++j)
      {
        putchar(' ');
      }
      blob_print(st->nodes[i].u.b);
    }
    else if(st->nodes[i].data_t.t == STACK_T)
    {
      stack_print(st->nodes[i].u.st, depth + 1, d_l);
    }
  }
  for(size_t i = 0; i < depth*d_l; ++i)
  {
    putchar(' ');
  }
  fputs(")\n", stdout);
}

#define M_PUSH(a) blob_stack_push(&st, a, &st_len, &st_alloc)

static void blob_stack_push
(
  uint8_t **data,
  uint8_t val,
  size_t *len,
  size_t *alloc
)
{
  if(*len == 0)
  {
    *data = PRV_MALLOC(sizeof(val) << 4);
    *alloc = sizeof(val) << 4;
    *len = 1;
    (*data)[0] = val;
    return;
  }
  if (*len == *alloc)
  {
    *data = PRV_REALLOC(*data, *alloc << 1);
    *alloc <<= 1;
  }
  (*data)[*len] = val;
  *len += 1;
}

blob *
blob_scan_fromstream
(
  FILE *stream
)
{
  size_t st_len = 0;
  size_t st_alloc;
  uint8_t *st = NULL;

  while(true)
  {
    const int fg = getc(stream);
    if(fg == EOF)
    {
      PRV_ERR_MACRO();
    }
    uint8_t c = fg;
    if(!isprint(fg))
    {
      PRV_ERR_MACRO();
    }
    switch(c)
    {
      case '\\':
        {
          int c2 = getc(stream);
          switch(c2)
          {
            case 'x':
              {
                int c3[2] =
                {
                  getc(stream),
                  getc(stream)
                };
                uint8_t tmp[2];

                for(size_t i = 0; i < 2; ++i)
                {
                  switch(c3[i])
                  {
                    case '0' ... '9':
                      tmp[i] = c3[i]-'0';
                      break;
                    case 'a' ... 'f':
                      tmp[i] = c3[i]+10-'a';
                      break;
                    case 'A' ... 'F':
                      tmp[i] = c3[i]+10-'A';
                      break;
                    default:
                      PRV_ERR_MACRO();
                  }
                }
                M_PUSH(tmp[1] | tmp[0] << 4);
              }
              break;
            case '\\':
              M_PUSH('\\');
              break;
            case 't':
              M_PUSH('\t');
              break;
            case 'n':
              M_PUSH('\n');
              break;
            case '"':
              M_PUSH('"');
              break;
            default:
              PRV_ERR_MACRO();
          }
        }
        break;
//      case '\t':
//      case '\n':
//        PRV_ERR_MACRO();
//        break;
      case '"':
        goto end;
      default:
        M_PUSH(c);
    }
  }

end:
  ;
  blob *tmp = blob_init(st_len, st);
  PRV_FREE(st);
  return tmp;
}



blob *
blob_scan_fromstream_word
(
  FILE *stream,
  const uint8_t f_c
)
{
  size_t st_len = 0;
  size_t st_alloc;
  uint8_t *st = NULL;
  M_PUSH(f_c);
  while(true)
  {
    const int fg = getc(stream);
    if(fg == EOF)
    {
      PRV_ERR_MACRO();
    }
    uint8_t c = fg;
    if(isspace(fg))
    {
      goto end;
    }
    if(!isprint(fg))
    {
      PRV_ERR_MACRO();
    }
    switch (c)
    {
      case '(':
      case ')':
        ungetc((int)c, stream);
        goto end;
      default:
        M_PUSH(c);
    }
  }

end:
  ;
  blob *tmp = blob_init(st_len, st);
  PRV_FREE(st);
  return tmp;
}

#undef M_PUSH



stack *
stack_scan_fromstream2
(
  FILE *stream
)
{
  while(true)
  {
    const int fg = getc(stream);
    if(fg == EOF)
    {
      PRV_ERR_MACRO();
    }
    if(isspace(fg))
    {
      continue;
    }
    if(fg == '(') // We need to have an opening bracket.
    {
      return stack_scan_fromstream_good(stream);
    }
    PRV_ERR_MACRO();
  }
}




#define M_PUSH(a) stack_push(&tmp, a)

stack *
stack_scan_fromstream_good
(
  FILE *stream
)
{
  stack *tmp = stack_init(0, NULL);

  while(true)
  {
    const int fg = getc(stream);
    if(fg == EOF)
    {
      PRV_ERR_MACRO();
    }
    if(isspace(fg))
    {
      continue;
    }

    uint8_t c = fg;
    switch (c)
    {
      case '"':
        M_PUSH
        (
          node_elm_init_b
          (
            blob_scan_fromstream(stream)
          )
        );
        break;
      case '(':
        M_PUSH
        (
          node_elm_init_st
          (
            stack_scan_fromstream_good(stream)
          )
        );
        break;
      case ')':
        goto end;
      default:
        if(isprint(c))
        {
          M_PUSH
          (
            node_elm_init_b
            (
              blob_scan_fromstream_word(stream,c)
            )
          );
        }
        else
        {
          //fprintf(stderr, "error: %c %d\n", fg, fg);
          PRV_ERR_MACRO();
        }
    }
  }
end:
  return tmp;
}

#undef M_PUSH

