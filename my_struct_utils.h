#pragma once

#include "my_struct.h"


void
blob_print
(
  blob *in
);


void
stack_print
(
  stack *st,
  size_t depth,
  size_t d_l
);


blob *
blob_scan_fromstream
(
  FILE *stream
);

/*
stack *
stack_scan_fromstream
(
  FILE *stream
);
*/

stack *
stack_scan_fromstream2
(
  FILE *stream
);


stack *
stack_scan_fromstream_good
(
  FILE *stream
);
