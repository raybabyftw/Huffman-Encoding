#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Counter.h"
#include "File.h"
#include "huffman.h"

struct search{
    char *token;
    char *code;
};

// Helper Function Prototypes
static struct huffmanTree *newNode(char *token, int freq);
static int compareNodes(const void *a, const void *b);
static int numLeaves(struct huffmanTree *root);
static void generateHuffmanTable(struct huffmanTree *root, char *codeBuffer, int depth, struct search *table, int *entryCounter);
static char *findCode(struct search *table, int numItems, char *token);
static int compareTokens(const void *a, const void *b);
static struct search *initHuffmanSearchTable(struct huffmanTree *root, int *tableSize);
static char *extendIfNecessary(char *encoded, int currentLen, int newLen, int *capacity);
static char *encodeTokens(File file, struct search *table, int tableSize);


// Task 1
// Decodes a given Huffman encoding using the provided Huffman tree and writes the decoded output to a file.
void decode(struct huffmanTree *tree, char *encoding, char *outputFilename) {
    if (!tree || !encoding) return;

    File outFile = FileOpenToWrite(outputFilename);
    if (!outFile) {
        fprintf(stderr, "Error opening file for writing: %s\n", outputFilename);
        return;
    }

    struct huffmanTree *currentNode = tree;

    for (int i = 0; encoding[i] != '\0'; ++i) {
        if (encoding[i] == '0') {
            currentNode = currentNode->left;
        } else if (encoding[i] == '1') {
            currentNode = currentNode->right;
        } else {
            fprintf(stderr, "Invalid character in encoding: %c\n", encoding[i]);
            FileClose(outFile);
            return;
        }

        if (!currentNode->left && !currentNode->right) {
            FileWrite(outFile, currentNode->token);
            currentNode = tree; 
        }
    }

    FileClose(outFile);
}

// Task 3
// The function builds a Huffman tree based on the frequency of tokens found in the file.
struct huffmanTree *createHuffmanTree(char *inputFilename) {
    Counter c = CounterNew();
    File file = FileOpenToRead(inputFilename);
    char token[MAX_TOKEN_LEN + 1]; 
    while (FileReadToken(file, token)) {
        CounterAdd(c, token);
    }
    FileClose(file);

    int numItems = CounterNumItems(c);
    struct item *items = CounterItems(c, &numItems);

    struct huffmanTree **nodes = malloc(numItems * sizeof(struct huffmanTree *));
    assert(nodes != NULL);
    for (int i = 0; i < numItems; i++) {
        nodes[i] = newNode(items[i].token, items[i].freq);
    }

    for (int i = 0; i < numItems; i++) {
        free(items[i].token);
    }
    free(items);

    while (numItems > 1) {
        qsort(nodes, numItems, sizeof(struct huffmanTree *), compareNodes);

        struct huffmanTree *left = nodes[0];
        struct huffmanTree *right = nodes[1];

        struct huffmanTree *combined = newNode(NULL, left->freq + right->freq);
        combined->left = left;
        combined->right = right;

        nodes[0] = combined;
        for (int i = 1; i < numItems - 1; i++) {
            nodes[i] = nodes[i + 1];
        }
        numItems--;
    }

    struct huffmanTree *root = nodes[0];
    free(nodes);
    
    CounterFree(c);
    
    return root;
}

// Helper Functions
// Creates a new huffmanTree node with the given token and frequency.
static struct huffmanTree *newNode(char *token, int freq) {
    struct huffmanTree *node = malloc(sizeof(struct huffmanTree));
    assert(node != NULL);
    node->token = (token != NULL) ? strdup(token) : NULL;
    node->freq = freq;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Compares two huffmanTree nodes based on their frequencies.
static int compareNodes(const void *a, const void *b) {
    struct huffmanTree *nodeA = *(struct huffmanTree **)a;
    struct huffmanTree *nodeB = *(struct huffmanTree **)b;
    return nodeA->freq - nodeB->freq;
}

// Task 4
// Encodes a given input file using the provided Huffman tree and returns the resulting Huffman encoded string.
char *encode(struct huffmanTree *root, char *inputFilename) {
    int tableSize;
    struct search *table = initHuffmanSearchTable(root, &tableSize);
    if (!table) {
        return NULL;
    }
    File tokens = FileOpenToRead(inputFilename);
    char *encoded = encodeTokens(tokens, table, tableSize);
    FileClose(tokens);

    for (int i = 0; i < tableSize; i++) {
        free(table[i].code);
    }
    free(table);

    return encoded;
}

// Helper Functions
// Returns the number of leaves in a tree
static int numLeaves(struct huffmanTree *root) {
    if (!root) return 0;
    if (!root->left && !root->right) return 1;
    return numLeaves(root->left) + numLeaves(root->right);
}

// Recursively traverses the Huffman tree to generate a Huffman encoding table from the tree.
static void generateHuffmanTable(struct huffmanTree *root, char *codeBuffer, int depth, struct search *table, int *entryCounter) {
    if (!root) return;

    if (!root->left && !root->right) {
        codeBuffer[depth] = '\0';
        table[*entryCounter].token = root->token;
        table[*entryCounter].code = strdup(codeBuffer);
        (*entryCounter)++;
        return;
    }

    codeBuffer[depth] = '0';
    generateHuffmanTable(root->left, codeBuffer, depth + 1, table, entryCounter);

    codeBuffer[depth] = '1';
    generateHuffmanTable(root->right, codeBuffer, depth + 1, table, entryCounter);
}

// Searches for a given token's Huffman code in a sorted Huffman search table using a binary search algorithm.
static char *findCode(struct search *table, int numItems, char *token) {
    int low = 0, high = numItems - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        int cmp = strcmp(table[mid].token, token);
        
        if (cmp == 0) return table[mid].code;
        if (cmp < 0) low = mid + 1;
        else high = mid - 1;
    }
    return NULL;
}

// Compares two tokens
static int compareTokens(const void *a, const void *b) {
    return strcmp(((struct search *)a)->token, ((struct search *)b)->token);
}

// Initializes a Huffman search table based on the given Huffman tree, where each entry in the table maps tokens to their corresponding Huffman codes.
static struct search *initHuffmanSearchTable(struct huffmanTree *root, int *tableSize) {
    int totalLeaves = numLeaves(root);
    struct search *table = (struct search *)malloc(totalLeaves * sizeof(struct search));
    assert(table != NULL);
    *tableSize = 0;
    char code[1024] = {0};
    generateHuffmanTable(root, code, 0, table, tableSize);
    qsort(table, *tableSize, sizeof(struct search), compareTokens);
    return table;
}

// Extends the buffer used to store Huffman-encoded data if its current length plus the new length exceeds the current capacity.
static char *extendIfNecessary(char *encoded, int currentLen, int newLen, int *capacity) {
    while (currentLen + newLen + 1 > *capacity) {
        *capacity *= 2;
        encoded = (char *)realloc(encoded, *capacity);
        if (!encoded) {
            return NULL;
        }
    }
    return encoded;
}

// Encodes tokens from a file using a Huffman search table and returns the resulting Huffman-encoded string.
static char *encodeTokens(File file, struct search *table, int tableSize) {
    int encodedLen = 0;
    int encodedCap = 512;
    char *encoded = (char *)malloc(encodedCap);
    if (!encoded) {
        return NULL;
    }
    char array[MAX_TOKEN_LEN];
    while (FileReadToken(file, array) == true) {
        char *tokenCode = findCode(table, tableSize, array);
        if (tokenCode) {
            encoded = extendIfNecessary(encoded, encodedLen, strlen(tokenCode), &encodedCap);
            if (!encoded) {
                return NULL;
            }
            strcpy(encoded + encodedLen, tokenCode);
            encodedLen += strlen(tokenCode);
        }
    }
    return (char *)realloc(encoded, encodedLen + 1);
}


