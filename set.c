/**
 * @author: Alexander Mark Thompson
 * @title: CS 201 Fall 2017 Assignment 3 Disjoint Set
 */

#include <stdio.h>
#include <stdlib.h>

#include "set.h"
#include "da.h"
#include "integer.h"

typedef struct NODE NODE;

struct set {
  DA *list;
  int numReps;
  void (*display)(FILE *, void *);
};

struct NODE {
  void *value;
  int index;
  struct NODE *parent;
};

static NODE *newNODE(void *, NODE *, int);
static void printNODE(FILE *, NODE *);

SET *newSET(void (*d)(FILE *, void *)) {
  SET *set = malloc(sizeof(struct set));
  set->list = newDA(d);
  set->numReps = 0;
  set->display = d;

  return set;
}

int makeSET(SET *d, void *value) {
  NODE *valNode = newNODE(value, NULL, sizeDA(d->list));
  insertDA(d->list, valNode);
  d->numReps += 1;

  return (sizeDA(d->list) - 1);
}

int findSET(SET *d, int index) {
  //returns the index of the representative of the value at the given index
  NODE *foundNode = getDA(d->list, index);
  NODE *parent = foundNode->parent;

  if (getINTEGER(foundNode->value) == getINTEGER(parent->value))
    return index;

  return findSET(d, parent->index);
}

int unionSET(SET *d, int index1, int index2) {
  int rep1 = findSET(d, index1);
  int rep2 = findSET(d, index2);

  if (rep1 == rep2)
    return 0;

//  NODE *node1 = getDA(d->list, index1);
  NODE *node2 = getDA(d->list, rep2);

  node2->parent = getDA(d->list, rep1);

  d->numReps -= 1;
  return 1;
}

int countSET(SET *d) {
  return d->numReps;
}


int displaySET(FILE *fp, SET *d) {
  int i;
  int size = sizeDA(d->list);
  for (i = 0; i < size; i++) {
    fprintf(fp, "%d:", i);
    printNODE(fp, getDA(d->list, i));
    fprintf(fp, "\n");
  }

  return 0;
}


/******************************************************************************/
/***                         Private Functions                              ***/
/******************************************************************************/
static NODE *newNODE(void *value, NODE *parent, int index) {
  NODE *node = malloc(sizeof(struct NODE));
  node->value = value;
  node->index = index;
  if (parent == NULL)
    node->parent = node;
  else
    node->parent = parent;

  return node;
}

static void printNODE(FILE *fp, NODE *node) {
  fprintf(fp, " %d", getINTEGER(node->value));
  if (node->parent->index != node->index) {
    printNODE(fp, node->parent);
  }
}