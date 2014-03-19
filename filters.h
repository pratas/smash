#ifndef FILTERS_H_INCLUDED
#define FILTERS_H_INCLUDED

#include "defs.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

typedef struct
  {
  uint64_t  pos;
  double    value;
  }
WEntry;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char *FilterSequence(char *, Parameters *);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif