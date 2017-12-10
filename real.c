#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "real.h"

struct REAL {
  double value;
};

REAL *newREAL(double a) {
  REAL *ptr = malloc( sizeof(REAL) );
  assert(ptr != 0);
  ptr->value = a;
  return ptr;
}

double getREAL(REAL *r) {
  return r->value;
}

double setREAL(REAL *r, double a) {
  int removed_val = r->value;
  r->value = a;
  return removed_val;
}

void displayREAL(FILE *fp, void *v) {
  fprintf(fp, "%lf", getREAL((REAL *)  v));
}

void freeREAL(REAL *r) {
  free(r);
}