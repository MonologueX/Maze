#define _CRT_SECURE_NO_WARNINGS 1

#include<stdio.h>
#include<assert.h>
#include <windows.h>

#define MAX_SIZE 10

typedef struct Point
{
	int row;
	int col;
}Point;
typedef Point DataType;

typedef struct Stack
{
	DataType _array[MAX_SIZE];
	int _size;
}Stack;

void StackInit(Stack* s)
{
	assert(s);
	s->_size = 0;
}

void StackPush(Stack* s, DataType data)
{
	assert(s);
	if (s->_size == MAX_SIZE)
		return;
	s->_array[s->_size++] = data;
}

void StackPop(Stack* s)
{
	assert(s);
	if (StackEmpty(s))
	{
		return;
	}
	s->_size--;
}

DataType StackTop(Stack* s)
{
	assert(s);
	return s->_array[s->_size - 1];
}

int StackSize(Stack* s)
{
	assert(s);
	return s->_size;
}

int StackEmpty(Stack* s)
{
	assert(s);
	return 0 == s->_size;
}