/**
 * @author: Alexander Mark Thompson
 * @title: CS 201 Fall 2017 Assignment 3 Red Black Tree
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "rbt.h"
#include "bst.h"
#include "queue.h"

typedef struct RBTNODE RBTNODE;

static RBTNODE *newRBTNODE(void *, void (*)(FILE *, void *), int (*)(void *, void *));
static void displayRBTNODE(FILE *, void *);
static void swapRBTNODE(BSTNODE *, BSTNODE *);
static int compareRBTNODE(void *, void *);
static char getColor(BSTNODE *);
static void setColor(BSTNODE *, char);

static int findMinDepthRBT(BSTNODE *);
static int findMaxDepthRBT(BSTNODE *);

static bool nodesAreEqual(BSTNODE *, BSTNODE *);
static bool isLeftChild(BSTNODE *);
static bool nodesAreLinear(BSTNODE *, BSTNODE *);
static void leftRotate(BST *, BSTNODE *);
static void rightRotate(BST *, BSTNODE *);
static void rotate(BST *, BSTNODE *, BSTNODE *);

static BSTNODE *getSibling(BSTNODE *);
static BSTNODE *getUncle(BSTNODE *);
static BSTNODE *getGrandParent(BSTNODE *);
static BSTNODE *getNephew(BSTNODE *);
static BSTNODE *getNiece(BSTNODE *);

static void insertionFixUp(BST *, BSTNODE *);
static void deletionFixUp(BST *, BSTNODE *);

struct rbt {
    struct bst *tree;
    int totalWords;
    void (*display)(FILE *, void *);
    int (*comparator)(void *, void *);
};

struct RBTNODE {
  int frequency;
  char color;
  void *value;    // Holds STRING/REAL/INTEGER
  void (*display)(FILE *, void *);
  int (*comparator)(void *, void *);
};

RBT *newRBT(
  void (*d)(FILE *, void *),
  int (*c)(void *, void *)
)
{
    RBT *t = malloc(sizeof(struct rbt));
    t->tree = newBST(displayRBTNODE, compareRBTNODE, swapRBTNODE);
    t->display = d;
    t->comparator = c;
    t->totalWords = 0;

    return t;
}

void insertRBT(RBT *t, void *value) {
  RBTNODE *nodeToInsert = newRBTNODE(value, t->display, t->comparator);

  BSTNODE *find = findBST(t->tree, nodeToInsert);

  if (find) {
    RBTNODE *foundNode = getBSTNODE(find);
    foundNode->frequency += 1;
  }
  else {
    insertBST(t->tree, nodeToInsert);
    BSTNODE *insertedNode = findBST(t->tree, nodeToInsert);
    insertionFixUp(t->tree, insertedNode);

  }
  t->totalWords += 1;
}

int findRBT(RBT *t, void *value) {
  RBTNODE *valueNode = newRBTNODE(value, t->display, t->comparator);
  RBTNODE *p = getBSTNODE(findBST(t->tree, valueNode));

  if (p == NULL) {
    return 0;
  }
  else {
    return p->frequency;
  }
}


void deleteRBT(RBT *t, void *value) {
  RBTNODE *nodeToDelete = newRBTNODE(value, t->display, t->comparator);

  BSTNODE *find = findBST(t->tree, nodeToDelete);

  if (find != NULL) {
    RBTNODE *decrementNode = getBSTNODE(find);

    decrementNode->frequency -= 1;

    if (decrementNode->frequency <= 0) {
      BSTNODE *leaf = swapToLeafBST(t->tree, find);
      deletionFixUp(t->tree, leaf);
      pruneLeafBST(t->tree, leaf);
    }
    t->totalWords -= 1;
  }
  else {
    printf("Value ");
    t->display(stdout, value);
    printf(" not found.\n");
  }
}

int sizeRBT(RBT *t) {
  return sizeBST(t->tree);
}

int wordsRBT(RBT *t) {
  return t->totalWords;     
}

void statisticsRBT(FILE *fp, RBT *t) {
  fprintf(fp, "Words/Phrases: %d\n", t->totalWords);
  fprintf(fp, "Nodes: %d\n", sizeBST(t->tree));
  fprintf(fp, "Minimum depth: %d\n", findMinDepthRBT(getBSTroot(t->tree)));    
  fprintf(fp, "Maximum depth: %d\n", findMaxDepthRBT(getBSTroot(t->tree)));    

}

void displayRBT(FILE *fp, RBT *t) {
  displayBST(fp, t->tree);
  fprintf(fp, "\n");
}

static RBTNODE *newRBTNODE(void *value, void (*d)(FILE *, void *), int (*c)(void *, void *)) {
  RBTNODE *node = malloc(sizeof(struct RBTNODE));
  node->frequency = 1;
  node->color = 'R';
  node->value = value;
  node->display = d;
  node->comparator = c;

  return node;
}

static void displayRBTNODE(FILE *fp, void *value) {
  RBTNODE *node = value;
  node->display(fp, node->value);

  if (node->frequency > 1) {
    fprintf(fp, "-%d", node->frequency);
  }
  fprintf(fp, "-");
  fprintf(fp, "%c", node->color);
}

static void swapRBTNODE(BSTNODE *n1, BSTNODE *n2) {
  RBTNODE *ra = getBSTNODE(n1);
  RBTNODE *rb = getBSTNODE(n2);

  void *vtemp = ra->value;
  ra->value = rb->value;
  rb->value = vtemp;

  int ctemp = ra->frequency;
  ra->frequency = rb->frequency;
  rb->frequency = ctemp;
}

static int compareRBTNODE(void *v1, void *v2) {
  RBTNODE *node1 = v1;
  RBTNODE *node2 = v2;

  if (node1 == NULL) {
    if (node2 == NULL)
      return 0;
    else
      return -1;
  }
  if (node2 == NULL) {
    return 1;
  }

  return node1->comparator(node1->value, node2->value);
}

static char getColor(BSTNODE *node) {
  if (node == NULL)
    return 'B';
  else {
    RBTNODE *valNode = getBSTNODE(node);
    return valNode->color;
  }

}

static void setColor(BSTNODE *node, char color) {
  if (node == NULL)
    return;

  RBTNODE *valNode = getBSTNODE(node);

  valNode->color = color;
}

static int findMinDepthRBT(BSTNODE *root) {
  if (root == NULL)
    return 0;

  int Lmin = findMinDepthRBT(getBSTNODEleft(root));
  int Rmin = findMinDepthRBT(getBSTNODEright(root));

  return (Lmin<Rmin?Lmin:Rmin) + 1;
}

static int findMaxDepthRBT(BSTNODE *root) {
  if (root == NULL)
    return 0;

  int Ldepth = findMaxDepthRBT(getBSTNODEleft(root));
  int Rdepth = findMaxDepthRBT(getBSTNODEright(root));

  return (Ldepth>Rdepth?Ldepth:Rdepth) + 1;
}

static bool nodesAreEqual(BSTNODE *n1, BSTNODE *n2) {
  if (n1 == NULL) {
    if (n2 == NULL)
      return true;
    else
      return false;
  }
  if (n2 == NULL)
    return false;

  RBTNODE *node1 = getBSTNODE(n1);
  RBTNODE *node2 = getBSTNODE(n2);

  if (node1->comparator(node1->value, node2->value) == 0)
    return true;
  else
    return false;
}

static bool isLeftChild(BSTNODE *x) {
  BSTNODE *parent = getBSTNODEparent(x);

  if (nodesAreEqual(getBSTNODEleft(parent), x))
    return true;
  else
    return false;
}

static bool nodesAreLinear(BSTNODE *x, BSTNODE *parent) {
  if (isLeftChild(parent) && isLeftChild(x))
    return true;
  else if (!isLeftChild(parent) && !isLeftChild(x))
    return true;
  else
    return false;
}

static void leftRotate(BST *tree, BSTNODE *x) {
  BSTNODE *y = getBSTNODEright(x);
  BSTNODE *root = getBSTroot(tree);

  if (nodesAreEqual(x, root)) {
    setBSTNODEparent(y, y);
    setBSTNODEparent(getBSTNODEleft(y), x);
    setBSTNODEright(x, getBSTNODEleft(y));
    setBSTNODEleft(y, x);
    setBSTNODEparent(x, y);
    setBSTroot(tree, y);
  }
  else if (nodesAreEqual(x, getBSTNODEright(getBSTNODEparent(x)))) {
    setBSTNODEright(getBSTNODEparent(x), y);
    setBSTNODEparent(y, getBSTNODEparent(x));
    setBSTNODEright(x, getBSTNODEleft(y));
    setBSTNODEparent(getBSTNODEleft(y), x);
    setBSTNODEleft(y, x);
    setBSTNODEparent(x, y);
  }
  else {
    setBSTNODEleft(getBSTNODEparent(x), y);
    setBSTNODEparent(y, getBSTNODEparent(x));
    setBSTNODEright(x, getBSTNODEleft(y));
    setBSTNODEparent(getBSTNODEleft(y), x);
    setBSTNODEleft(y, x);
    setBSTNODEparent(x, y);
  }

}

static void rightRotate(BST *tree, BSTNODE *x) {
  BSTNODE *y = getBSTNODEleft(x);
  BSTNODE *root = getBSTroot(tree);

  if (nodesAreEqual(x, root)) {
    setBSTNODEparent(y, y);
    setBSTNODEparent(getBSTNODEright(y), x);
    setBSTNODEleft(x, getBSTNODEright(y));
    setBSTNODEright(y, x);
    setBSTNODEparent(x, y);
    setBSTroot(tree, y);
  }
  else if (nodesAreEqual(x, getBSTNODEleft(getBSTNODEparent(x)))) {
    setBSTNODEleft(getBSTNODEparent(x), y);
    setBSTNODEparent(y, getBSTNODEparent(x));
    setBSTNODEleft(x, getBSTNODEright(y));
    setBSTNODEparent(getBSTNODEright(y), x);
    setBSTNODEright(y, x);
    setBSTNODEparent(x, y);
  }
  else {
    setBSTNODEright(getBSTNODEparent(x), y);
    setBSTNODEparent(y, getBSTNODEparent(x));
    setBSTNODEleft(x, getBSTNODEright(y));
    setBSTNODEparent(getBSTNODEright(y), x);
    setBSTNODEright(y, x);
    setBSTNODEparent(x, y);
  }

}

static void rotate(BST *tree, BSTNODE *x, BSTNODE *parent) {
  if (nodesAreEqual(x, getBSTNODEleft(parent))) {
    rightRotate(tree, parent);
  }
  else {
    leftRotate(tree, parent);
  }
}

static BSTNODE *getSibling(BSTNODE *node) {
  if (node == NULL)
    return NULL;

  BSTNODE *parent = getBSTNODEparent(node);

  if (nodesAreEqual(parent, node))
    return NULL;

  if (nodesAreEqual(node, getBSTNODEright(parent)))
    return getBSTNODEleft(parent);
  else
    return getBSTNODEright(parent);
}

static BSTNODE *getUncle(BSTNODE *node) {
  if (node == NULL)
    return NULL;

  BSTNODE *parent = getBSTNODEparent(node);

  if (nodesAreEqual(parent, node))
    return NULL;

  return getSibling(parent);
}

static BSTNODE *getGrandParent(BSTNODE *node) {
  if (node == NULL)
    return NULL;

  BSTNODE *parent = getBSTNODEparent(node);

  if (nodesAreEqual(parent, node))
    return NULL;
  if (nodesAreEqual(parent, getBSTNODEparent(parent)))
    return NULL;

  return getBSTNODEparent(parent);
}

static BSTNODE *getNephew(BSTNODE *node) {
  if (node == NULL)
    return NULL;

  if (isLeftChild(node))
    return getBSTNODEright(getSibling(node));
  else
    return getBSTNODEleft(getSibling(node));
}

static BSTNODE *getNiece(BSTNODE *node) {
  if (node == NULL)
    return NULL;

  if (isLeftChild(node))
    return getBSTNODEleft(getSibling(node));
  else
    return getBSTNODEright(getSibling(node));
}

static void insertionFixUp(BST *t, BSTNODE *x) {
  while (1) {
    BSTNODE *parent = getBSTNODEparent(x);
    BSTNODE *uncle = getUncle(x);
    BSTNODE *grandParent = getGrandParent(x);

    if (nodesAreEqual(x, parent))
      break;
    if (getColor(parent) == 'B')
      break;

    if (getColor(uncle) == 'R') {
      setColor(parent, 'B');
      setColor(uncle, 'B');
      setColor(grandParent, 'R');

      x = grandParent;
    }
    else {

      if (!nodesAreLinear(x, parent)) {
        BSTNODE *oldparent = parent;
        
        BSTNODE *oldx = x;

        rotate(t, x, parent);           
        
        x = oldparent;
        parent = oldx;
      }

      setColor(parent, 'B');
      setColor(grandParent, 'R');

      rotate(t, parent, grandParent);

      break;
    }
  }
  
  BSTNODE *root = getBSTroot(t);
  setColor(root, 'B');
}

static void deletionFixUp(BST *t, BSTNODE *x) {
  while (1) {
    BSTNODE *parent = getBSTNODEparent(x);
    BSTNODE *sibling = getSibling(x);
    BSTNODE *niece = getNiece(x);
    BSTNODE *nephew = getNephew(x);

    if (nodesAreEqual(x, parent))
      break;

    if (getColor(x) == 'R')
      break;

    if (getColor(sibling) == 'R') {
      setColor(parent, 'R');
      setColor(sibling, 'B');
      rotate(t, sibling, parent);
    }
    else if (getColor(nephew) == 'R') {
      setColor(sibling, getColor(parent));
      setColor(parent, 'B');
      setColor(nephew, 'B');
      rotate(t, sibling, parent);
      break;
    }
    else if (getColor(niece) == 'R') {
      setColor(niece, 'B');
      setColor(sibling, 'R');
      rotate(t, niece, sibling);
    }
    else {
      setColor(sibling, 'R');
      x = parent;
    }
  }
  setColor(x, 'B');
}