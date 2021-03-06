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

#include "my_global.h"
#include "my_sys.h"
#include "s_set.h"
#include <tr1/unordered_map>
#include <string>
#include "chobieton.h"

using namespace std;
using namespace std::tr1;

class Chobie_data
{
public:
	SkipList *current_table;
	SkipListNode *current_node;
	long long rank; 

	Chobie_data(void);
	~Chobie_data(void);

	int create_table(const char *path);
	int open_table(const char *path);
	int delete_table(const char *path);
	int rename_table(const char *from, const char *to);
	int read_row(uchar *buf, int length, long long position, SkipListNode *output);
	long long current_position();
	long long write_row(uchar *buf, int length, int score);
	long long current_rank();
	void clear_rank();
	int delete_row(const uchar *buf, int score);
	long long update_row(const uchar *old_rec, int old_score, uchar *new_rec, int score, int length, long long position);
};