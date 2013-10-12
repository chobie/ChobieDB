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
#include "s_set.h"
#define MAX_LEVEL 32

/* 
 * This skiplist implementation is almost re implementation of the redis's 
 * Skiplist implementation. As to understanding Skiplist algorithm and easy to re use.
 *
 * see also redis/src/t_zset.c
 *
 * Copyright (c) 2009-2012, Salvatore Sanfilippo <antirez at gmail dot com>
 * Copyright (c) 2009-2012, Pieter Noordhuis <pcnoordhuis at gmail dot com>
 *  All rights reserved.
 */

int create_skiplist_node(unsigned int level, double score, SkipListNode **output)
{
	SkipListNode *node = NULL;
	int i;
	node = (SkipListNode*)malloc(sizeof(*node)+level*sizeof(struct SkipListLevel));
	if (node == NULL) {
		fprintf(stderr, "[FATAL]");
		return -1;
	}
	for (i = 0; i < level; i++) {
		node->level[i].forward = NULL;
		node->level[i].span = 0;
	}

	node->score =  score;
	*output = node;
	return 0;
}

int create_skiplist(SkipList **output)
{
	SkipList *list = NULL;

	list = (SkipList*)malloc(sizeof(*list));
	list->header = NULL;
	list->tail = NULL;
	list->max_level = MAX_LEVEL;
	list->level = 1;
	list->length = 0;

	create_skiplist_node(MAX_LEVEL, 0, &list->header);
	assert(list->header);

	*output = list;
	return 0;
}

void  free_skiplist_node(SkipListNode *node)
{
	assert(node);
	free(node);
}

void  free_skiplist(SkipList *list)
{
	SkipListNode *node, *next;
	node = list->header->level[0].forward;

	assert(list);
	assert(node);
	free(list->header);
	while (node) {
		next = node->level[0].forward;
		//fprintf(stderr, "score; %lf\n", node->score);
		free_skiplist_node(node);
		node = next;
	}
	free(list);
}

int skiplist_get_random_level()
{
	int level = 1;

	while ((random()&0xFFFF) < (0.25 * 0xFFFF))
		level += 1;

	return (level<MAX_LEVEL) ? level : MAX_LEVEL;
}

int insert_skiplist(SkipList *list, double score)
{
	SkipListNode *update[MAX_LEVEL] = {0}, *node, *add;
	unsigned int rank[MAX_LEVEL] = {0};
	int i, level;

	assert(list);
	node = list->header;
	assert(node);

	for (i = list->level -1; i >= 0; i--) {
		while (node->level[i].forward &&
			(node->level[i].forward->score < score)) {
			rank[i] += node->level[i].span;
			node = node->level[i].forward;
		}
		update[i] = node;
	}

	level = skiplist_get_random_level();
	assert(level);
	if (level > list->level) {
		for (i = list->level; i < level; i++) {
			rank[i] = 0;
			update[i] = list->header;
			update[i]->level[i].span = list->length;
		}
		list->level = level;
	}

	create_skiplist_node(level, score, &add);
	for (i = 0; i < level; i++) {
		add->level[i].forward = update[i]->level[i].forward;
		update[i]->level[i].forward = add;

		add->level[i].span = update[i]->level[i].span - (rank[0] - rank[i]);
		update[i]->level[i].span = (rank[0] - rank[i]) + 1;
	}

	for (i = level; i < list->level; i++) {
		update[i]->level[i].span++;
	}

	list->length++;

	return 0;
}

static int delete_node(SkipList *list, SkipListNode *node, double score, SkipListNode **update)
{
	int i;
	for (i = 0; i < list->level; i++) {
		if (update[i]->level[i].forward == node) {
			update[i]->level[i].span += node->level[i].span - 1;
			update[i]->level[i].forward = node->level[i].forward;
		} else {
			update[i]->level[i].span -= 1;
		}
	}

	if (node->level[0].forward) {

	} else {

	}

	while (list->level >1 && list->header->level[list->level-1].forward == NULL) {
		list->level--;
	}
	list->length--;
	return 0;
}

int delete_skiplist_node(SkipList *list, double score)
{
	SkipListNode *update[MAX_LEVEL] = {0}, *node;
	int i = 0;

	node = list->header;
	for (i = list->level - 1; i >= 0; i--) {
		while (node->level[i].forward &&
			(node->level[i].forward->score < score)) {
			node = node->level[i].forward;
		}
		update[i] = node;
	}

	node = node->level[0].forward;
	if (node && score == node->score) {
		delete_node(list, node, score, update);
		free_skiplist_node(node);
		return 1;
	} else {
		return 0;
	}
}
