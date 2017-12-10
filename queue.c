/**
 * @author: Alexander Mark Thompson
 * @title: CS 201 Fall 2017 Assignment 3 Queue
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "queue.h"
#include "cda.h"

struct queue {
  CDA *array;
  void (*display)(FILE *, void *);
};

QUEUE *newQUEUE(void (*d)(FILE *, void *)) {
  assert( sizeof(QUEUE) != 0 );

  QUEUE *newQueue = malloc( sizeof(QUEUE) );
  newQueue->array = newCDA(d);
  newQueue->display = d;

  return newQueue;
}

void enqueue(QUEUE *items, void *value) {
  insertCDAback(items->array, value);
}

void *dequeue(QUEUE *items) {
  assert( sizeCDA(items->array) > 0 );
  return removeCDAfront(items->array);
}

void *peekQUEUE(QUEUE *items) {
  assert( sizeCDA(items->array) > 0 );
  return getCDA(items->array, 0);
}

int sizeQUEUE(QUEUE *items) {
  return sizeCDA(items->array);
}

void displayQUEUE(FILE* fp, QUEUE *items) {
  fprintf(fp, "<");

  if (sizeCDA(items->array) != 0) {
    int i;
    for (i = 0; i < sizeCDA(items->array); i++) {
      items->display(fp, getCDA(items->array, i));
      if (i != sizeCDA(items->array) - 1) { fprintf(fp, ","); }
    }
  }

  fprintf(fp, ">");
}

void visualizeQUEUE(FILE *fp, QUEUE *items) {
  displayCDA(fp, items->array);
}