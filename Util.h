#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

/*
DYNAMIC ARRAY USING A LINKED LIST
*/
typedef struct
{
	void* data;
	size_t capacity;
	size_t size;
} vector;

int init_vector(vector* v);
int push_back(vector* v, void* item);
void* get_element(vector* v, int index);
int remove_at(vector* v, int index);
int pop_back(vector* v);
int pop_front(vector* v);
int destroy_vector(vector* v);

/*
END
*/

#endif
