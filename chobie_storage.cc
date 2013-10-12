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

Chobie_data::Chobie_data(void)
{
	current_table = NULL;
}

Chobie_data::~Chobie_data(void)
{
}

int Chobie_data::create_table(const char *path)
{
	return 0;
}

int Chobie_data::open_table(const char *path)
{
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

	return 0;
}

int Chobie_data::delete_table(const char *path)
{
	Chobieton::erase(path);
	return 0;
}

int Chobie_data::rename_table(const char *from, const char *to)
{
	Chobieton::rename(from, to);
	return 0;
}
