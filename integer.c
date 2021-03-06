#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "integer.h"

struct INTEGER
    {
    int value;
    };

INTEGER *
newINTEGER(int x)
    {
    INTEGER *p = malloc(sizeof(INTEGER));
    assert(p != 0);
    p->value = x;
    return p;
    }

int
getINTEGER(INTEGER *v)
    {
    return v->value;
    }

int
setINTEGER(INTEGER *v,int x)
    {
    int old = v->value;
    v->value = x;
    return old;
    }

void
displayINTEGER(FILE *fp,void *v)
    {
    fprintf(fp,"%d",getINTEGER((INTEGER *) v));
    }

void
freeINTEGER(INTEGER *v)
    {
    free(v);
    }

int
compareINTEGER(void *n, void *n2) {
  int val1 = getINTEGER(n);
  int val2 = getINTEGER(n2);

  if (val1 < val2) return -1;
  else if (val1 > val2) return 1;
  else return 0;
}