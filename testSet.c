#include <stdio.h>
#include "set.h"
#include "integer.h"

static SET *initSet(void (*)(FILE *, void *));
static void testMakeSet(SET *, void *);
static void testFindSet(SET *, int);
static void testUnionSet(SET *, int, int);
static void testCountSet(SET *);
static void display(SET *);

int main(void) {
  SET *set = initSet(displayINTEGER);

  testMakeSet(set, newINTEGER(1));
  testMakeSet(set, newINTEGER(2));
  testMakeSet(set, newINTEGER(3));

  testCountSet(set);

  testFindSet(set, 0);

  testUnionSet(set, 0, 1);

  printf("Now validating union through findSET...\n\n");

  testFindSet(set, 0);
  testFindSet(set, 1);

  testCountSet(set);

  display(set);

  return 0;
}

static SET *initSet(void (*d)(FILE *, void *)) {
  printf("Attempting to initialize new set...\n");
  SET *set = newSET(d);
  printf("Init successful.\n\n");

  return set;
}

static void testMakeSet(SET *s, void *value) {
  INTEGER *val1 = value;
  int val = getINTEGER(val1);

  printf("Attempting to makeSET with value %d...\n", val);
  int index = makeSET(s, value);
  printf("makeSET successful, index is: %d.\n\n", index);
}

static void testFindSet(SET *s, int index) {
  printf("Attempting to return the index of the rep of the value at %d...\n", index);
  int found = findSET(s, index);
  printf("findSET successful, index of rep of value at %d is: %d.\n\n", index, found);
}

static void testUnionSet(SET *s, int index1, int index2) {
  printf("Attempting to union val at index1 (index1 is %d) and index2 (%d) :: \n", index1, index2);
  printf("The rep of val at index1 is: %d. The rep of val at index2 is: %d.\n", findSET(s, index1), findSET(s, index2));

  int good = unionSET(s, index1, index2);

  if (good == 0) {
    printf("Already have same rep!\n");
    return;
  }

  printf("Union successful, rep of val at index2's parent is now rep of val at index1's parent.\n\n");
}

static void testCountSet(SET *s) {
  int count = countSET(s);
  printf("Current number of representatives in the list is: %d.\n\n", count);
}

static void display(SET *s) {
  printf("Set is: \n");
  displaySET(stdout, s);
  printf("\n");
}