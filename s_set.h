/*
 * Copyright (c) 2013, Shuhei Tanuma <shuhei.tanuma@gmail.com>
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE. 
 */
#ifndef T_CHOBIE_SET
#define T_CHOBIE_SET
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

struct SkipListLevel {
    struct SkipListNode *forward;
    unsigned int span;
}; 

typedef struct SkipListNode {
    double score;
    void *data;
    size_t length;
    struct SkipListLevel level[];
} SkipListNode;

typedef struct SkipList {
    SkipListNode *header, *tail;
    unsigned long length;
    unsigned long max_level;
    int level;
} SkipList;


int create_skiplist_node(unsigned int level, double score, SkipListNode **output, void *data, size_t length);
int create_skiplist(SkipList **output);
void  free_skiplist_node(SkipListNode *node);
void  free_skiplist(SkipList *list);
int insert_skiplist(SkipList *list, double score, void *data, size_t length);
int delete_skiplist_node(SkipList *list, double score);
int update_skiplist_node(SkipList *list, double score, void *data, size_t length);
#endif