// Implementation of the Counter ADT

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Counter.h"

struct treeNode {
    char *token;
    int count;
    struct treeNode *left;
    struct treeNode *right;
};

struct counter {
    struct treeNode *root;
    int numItems;
};

// Helper Function Pzrototypes
static void freeTreeNode(struct treeNode *node);
static void recursiveFillItems(struct treeNode *node, struct item *items, int *index);

// Creates a new Counter object for counting and storing token occurrences.
Counter CounterNew(void) {
    Counter c = malloc(sizeof(struct counter));
    if (c == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }
    c->root = NULL;
    c->numItems = 0;
    return c;
}

// Frees the memory associated with a Counter object and its internal data.
void CounterFree(Counter c) {
    if (c) {
        freeTreeNode(c->root);
        free(c);
    }
}

// Adds a token to the Counter object, incrementing its count if the token already exists.
void CounterAdd(Counter c, char *token) {
    if (!c) return;

    struct treeNode **curr = &(c->root);

    while (*curr) {
        int cmp = strcmp(token, (*curr)->token);
        if (cmp == 0) {
            (*curr)->count++;
            return;
        }
        curr = (cmp < 0) ? &((*curr)->left) : &((*curr)->right);
    }

    *curr = malloc(sizeof(struct treeNode));
    if (*curr == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }
    (*curr)->token = strdup(token);
    (*curr)->count = 1;
    (*curr)->left = NULL;
    (*curr)->right = NULL;
    c->numItems++;
}

// Returns the number of unique tokens stored in the Counter.
int CounterNumItems(Counter c) {
    if (!c) return 0;
    return c->numItems;
}

// Retrieves the count of a specific token in the Counter.
int CounterGet(Counter c, char *token) {
    if (!c) return 0;

    struct treeNode *curr = c->root;
    while (curr) {
        int cmp = strcmp(token, curr->token);
        if (cmp == 0) return curr->count;
        curr = (cmp < 0) ? curr->left : curr->right;
    }
    return 0; 
}

// HELPER FUNCTIONS 

//  Recursively fills the given items array with token strings and their corresponding counts 
//  from the tree in an in-order traversal.
static void recursiveFillItems(struct treeNode *node, struct item *items, int *index) {
    if (!node) return;

    recursiveFillItems(node->left, items, index);
    items[*index].token = strdup(node->token);
    items[*index].freq = node->count;
    (*index)++;
    recursiveFillItems(node->right, items, index);
}

// Returns an array of items containing token strings and their corresponding counts from the tree.
struct item *CounterItems(Counter c, int *numItems) {
    if (!c) return NULL;

    struct item *items = malloc(c->numItems * sizeof(struct item));
    if (items == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    int index = 0;
    recursiveFillItems(c->root, items, &index);
    *numItems = c->numItems;
    return items;
}

// Recursively frees memory associated with the given treeNode and all its kids
static void freeTreeNode(struct treeNode *node) {
    if (node) {
        freeTreeNode(node->left);
        freeTreeNode(node->right);
        free(node->token);
        free(node);
    }
}