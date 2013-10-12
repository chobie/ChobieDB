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

#include "chobie_storage.h"
#include <my_dir.h>
#include <field.h>

Chobie_data::Chobie_data(void)
{
	current_table = NULL;
}

Chobie_data::~Chobie_data(void)
{
}

int Chobie_data::create_table(const char *path)
{
	DBUG_ENTER("Chobie_data::create_table");
	DBUG_RETURN(0);
}

int Chobie_data::open_table(const char *path)
{
	DBUG_ENTER("Chobie_data::open_table");
	string table_path = path;
	SkipList *list;
	unordered_map<string, SkipList*>::iterator i;  
	i = Chobieton::container.find(table_path);

	if (i == Chobieton::container.end()) {
		create_skiplist(&list);
		Chobieton::container.insert(make_pair(path, list));
		current_table = list;
	} else {
		current_table = i->second;
	}

	DBUG_RETURN(0);
}

int Chobie_data::delete_table(const char *path)
{
	DBUG_ENTER("Chobie_data::delete_table");
	Chobieton::erase(path);
	DBUG_RETURN(0);
}

int Chobie_data::rename_table(const char *from, const char *to)
{
	DBUG_ENTER("Chobie_data::rename_table");
	Chobieton::rename(from, to);
	DBUG_RETURN(0);
}

int Chobie_data::read_row(uchar *buf, int length, long long position)
{
	SkipListNode *node;
	DBUG_ENTER("Chobie_data::read_row");
	if (position == -1) {
		DBUG_RETURN(-1);
	}
	if (position <= 0) {
		position =  (long long)current_table->header->level[0].forward;
	}

	node = (SkipListNode*)position;
	if (node == NULL) {
		DBUG_RETURN(HA_ERR_END_OF_FILE);
	} else {
		assert(node);
		memcpy(buf, node->data, node->length);

		if (node->level[0].forward != NULL) {
			node = node->level[0].forward;
			current_node = node;
		} else {
			current_node = NULL;
		}
	}

	DBUG_RETURN(0);
}

long long Chobie_data::current_position()
{
	if (current_node != NULL) {
		return (long long)current_node;
	} else {
		return -1;
	}
}

long long Chobie_data::write_row(uchar *buf, int length, int score)
{
	uchar *copy = {0};
	copy = (uchar*)my_malloc(length, MYF(MY_ZEROFILL | MY_WME));
	memcpy(copy, buf, length);
	insert_skiplist(current_table, score, (void*)copy, length);
	return 0;
}