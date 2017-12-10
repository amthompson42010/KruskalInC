/**
 * @author: Alexander Mark Thompson
 * @title: CS 201 Fall 2017 Assignment 3 Circular Dynamic Array
 */

#ifndef __CDA_INCLUDED__
#define __CDA_INCLUDED__

#include <stdio.h>

typedef struct cda CDA;

extern CDA *newCDA(void (*d)(FILE *,void *));
extern void insertCDAfront(CDA *items,void *value);
extern void insertCDAback(CDA *items,void *value);
extern void *removeCDAfront(CDA *items);
extern void *removeCDAback(CDA *items);
extern void unionCDA(CDA *recipient,CDA *donor);
extern void *getCDA(CDA *items,int index);
extern void *setCDA(CDA *items,int index,void *value);
extern void **extractCDA(CDA *items);
extern int sizeCDA(CDA *items);
extern void visualizeCDA(FILE *,CDA *items);
extern void displayCDA(FILE *,CDA *items);

#endif