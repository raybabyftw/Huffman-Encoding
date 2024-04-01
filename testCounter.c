// Main program for testing the Counter ADT

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Counter.h"

static void test1(void);
static void test2(void);
static void test3(void);
static void test4(void);
static void test5(void);
static void test6(void);
static void test7(void);
static void test8(void);
static void test9(void);
static void test10(void);
static void test11(void);


int main(void) {
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();
    test9();
    test10();
    test11();
}

static void test1(void) {
    Counter counter = CounterNew();

    char *tokens[10] = {
        "a", "dog", "cat", "a", "bird", "dog", "a", "a", "cat", "dog",
    };

    char token[100];
    for (int i = 0; i < 10; i++) {
        strcpy(token, tokens[i]);
        CounterAdd(counter, token);
    }

    assert(CounterNumItems(counter) == 4);

    CounterFree(counter);

    printf("Test 1 passed!\n");
}

static void test2(void) {
    Counter counter = CounterNew();

    char *tokens[10] = {
        "a", "dog", "cat", "a", "bird", "dog", "a", "a", "cat", "dog"
    };

    char token[100];
    for (int i = 0; i < 10; i++) {
        strcpy(token, tokens[i]);
        CounterAdd(counter, token);
    }

    assert(CounterGet(counter, "a") == 4);
    assert(CounterGet(counter, "bird") == 1);
    assert(CounterGet(counter, "cat") == 2);
    assert(CounterGet(counter, "dog") == 3);
    assert(CounterGet(counter, "emu") == 0);

    CounterFree(counter);

    printf("Test 2 passed!\n");
}

static void test3(void) {
    Counter counter = CounterNew();

    char *tokens[10] = {
        "a", "dog", "cat", "a", "bird", "dog", "a", "a", "cat", "dog"
    };

    char token[100];
    for (int i = 0; i < 10; i++) {
        strcpy(token, tokens[i]);
        CounterAdd(counter, token);
    }

    int numItems = 0;
    struct item *items = CounterItems(counter, &numItems);
    assert(numItems == 4);

    // Simple sort to put items in alphabetical order
    for (int i = 0; i < numItems; i++) {
        int min = i;
        for (int j = i + 1; j < numItems; j++) {
            if (strcmp(items[j].token, items[min].token) < 0) {
                min = j;
            }
        }

        struct item tmp = items[i];
        items[i] = items[min];
        items[min] = tmp;
    }

    assert(strcmp(items[0].token, "a") == 0 && items[0].freq == 4);
    assert(strcmp(items[1].token, "bird") == 0 && items[1].freq == 1);
    assert(strcmp(items[2].token, "cat") == 0 && items[2].freq == 2);
    assert(strcmp(items[3].token, "dog") == 0 && items[3].freq == 3);

    for (int i = 0; i < numItems; i++) {
        free(items[i].token);
    }
    free(items);
    CounterFree(counter);

    printf("Test 3 passed!\n");
}

// Test for an empty counter
static void test4(void) {
    Counter counter = CounterNew();

    assert(CounterNumItems(counter) == 0);
    assert(CounterGet(counter, "a") == 0);

    CounterFree(counter);

    printf("Test 4 passed!\n");
}

// Test for adding an empty token
static void test5(void) {
    Counter counter = CounterNew();

    CounterAdd(counter, "");
    assert(CounterNumItems(counter) == 1);
    assert(CounterGet(counter, "") == 1);

    CounterFree(counter);

    printf("Test 5 passed!\n");
}

// Test for querying a non-existent token
static void test6(void) {
    Counter counter = CounterNew();

    assert(CounterGet(counter, "nonExistent") == 0);

    CounterFree(counter);

    printf("Test 6 passed!\n");
}

// Test for a single token added multiple times
static void test7(void) {
    Counter counter = CounterNew();

    for (int i = 0; i < 100; i++) {
        CounterAdd(counter, "repeated");
    }

    assert(CounterGet(counter, "repeated") == 100);

    CounterFree(counter);

    printf("Test 7 passed!\n");
}

// Test for mixed case tokens
static void test8(void) {
    Counter counter = CounterNew();

    CounterAdd(counter, "Token");
    CounterAdd(counter, "token");
    CounterAdd(counter, "TOKEN");

    assert(CounterGet(counter, "Token") == 1);
    assert(CounterGet(counter, "token") == 1);
    assert(CounterGet(counter, "TOKEN") == 1);

    CounterFree(counter);

    printf("Test 8 passed!\n");
}

// Test for tokens with whitespace
static void test9(void) {
    Counter counter = CounterNew();

    CounterAdd(counter, " leadingSpace");
    CounterAdd(counter, "trailingSpace ");
    CounterAdd(counter, "middle space");

    assert(CounterGet(counter, " leadingSpace") == 1);
    assert(CounterGet(counter, "trailingSpace ") == 1);
    assert(CounterGet(counter, "middle space") == 1);

    CounterFree(counter);

    printf("Test 9 passed!\n");
}

// Test for freeing an empty counter
static void test10(void) {
    Counter counter = CounterNew();

    CounterFree(counter);

    printf("Test 10 passed!\n");
}

// Test for multiple calls to CounterAdd for the same token
static void test11(void) {
    Counter counter = CounterNew();

    CounterAdd(counter, "apple");
    CounterAdd(counter, "apple");
    CounterAdd(counter, "apple");

    assert(CounterGet(counter, "apple") == 3);

    CounterFree(counter);

    printf("Test 11 passed!\n");
}