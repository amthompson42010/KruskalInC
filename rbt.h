/**
 * @author: Alexander Mark Thompson
 * @title: CS 201 Fall 2017 Assignment 3 Red Black Tree
 */

#ifndef __RBT_INCLUDED__
#define __RBT_INCLUDED__

#include <stdio.h>

typedef struct rbt RBT;

extern RBT *newRBT(
    void (*)(FILE *,void *),           
    int (*)(void *,void *));           
extern void insertRBT(RBT *,void *);
extern int findRBT(RBT *,void *);
extern void deleteRBT(RBT *,void *);
extern int sizeRBT(RBT *);
extern int wordsRBT(RBT *);
extern void statisticsRBT(FILE *,RBT *);
extern void displayRBT(FILE *,RBT *);

#endif