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

#include "chobieton.h"

unordered_map<string, SkipList*> Chobieton::container;

int Chobieton::rename(const char *from, const char *to)
{
	string table_path = from;
	string to_path = to;

	unordered_map<string, SkipList*>::iterator i;  
	i = container.find(table_path);

	if (i != container.end()) {
		container.insert(make_pair(to_path, i->second));
		container.erase(table_path);
	}


	return 0;
}

int Chobieton::erase(const char *name)
{
	string table_path = name;
	container.erase(table_path);
	return 0;
}