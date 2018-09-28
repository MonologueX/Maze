#define _CRT_SECURE_NO_WARNINGS 1
/////////////////////////////////
//头文件
/////////////////////////////////
#include<stdio.h>
#include <windows.h>
#include"Stack.h"
#define MAX_ROW 6
#define MAX_COL 6
#define TEST_HEADER printf("\n============%s============\n",__FUNCTION__)

typedef struct Maze
{
	int map[MAX_ROW][MAX_COL];
}Maze;

//typedef struct Point
//{
//	int row;
//	int col;
//}Point;

/////////////////////////////////
//共用代码
/////////////////////////////////

void InitMaze(Maze* maze)
{
	int map[MAX_ROW][MAX_COL] = {
		{ 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 0, 0, 0 },
		{ 0, 1, 1, 1, 1, 0 },
		{ 0, 1, 0, 1, 1, 0 },
		{ 0, 1, 1, 1, 1, 1 },
		{ 0, 1, 0, 1, 0, 0 },
	};
	int i = 0;
	for (; i < MAX_ROW; ++i)
	{
		int j = 0;
		for (; j < MAX_COL; ++j)
		{
			maze->map[i][j] = map[i][j];
		}
	}
	return;
}

void PrintMaze(Maze* maze)
{
	int i = 0;
	for (; i < MAX_ROW; ++i)
	{
		printf("\t");
		int j = 0;
		for (; j < MAX_COL; ++j)
		{
			printf("%2d ", maze->map[i][j]);
		}
		printf("\n");
	}
}

int IsValidEntry(Maze* maze, Point entry)
{
	if (entry.row == 0 || entry.row == MAX_ROW - 1 ||
		entry.col == 0 || entry.col == MAX_COL - 1)
	{
		if (maze->map[entry.row][entry.col] == 1)
		{
			return 1;
		}
	}
	return 0;
}

int IsPass(Maze* maze, Point cur)
{
	return 1 == maze->map[cur.row][cur.col];
}

int IsExit(Maze*maze, Point cur, Point entry)
{
	if (cur.col == entry.col && cur.row == entry.row)
		return 0;
	if (cur.row == 0 || cur.row == MAX_ROW - 1 ||
		cur.col == 0 || cur.col == MAX_COL - 1)
	{
		return 1;
	}
	return 0;
}

/////////////////////////////////
//1.简单非递归
/////////////////////////////////

void _PassMazeNor(Maze*maze, Point entry, Stack* s)
{
	if (!IsValidEntry(maze, entry))
		return;
	StackInit(s);
	StackPush(s, entry);
	while (!StackEmpty(s))
	{
		Point next, pos;
		pos = StackTop(s);
		maze->map[pos.row][pos.col] = 2;

		if (IsExit(maze, pos, entry))
		{
			return;
		}

		//up
		next = pos;
		next.row -= 1;
		if (IsPass(maze, next))
		{
			StackPush(s, next);
			continue;
		}
		//left
		next = pos;
		next.col -= 1;
		if (IsPass(maze, next))
		{
			StackPush(s, next);
			continue;
		}
		//right
		next = pos;
		next.col += 1;
		if (IsPass(maze, next))
		{
			StackPush(s, next);
			continue;
		}
		//down
		next = pos;
		next.row += 1;
		if (IsPass(maze, next))
		{
			StackPush(s, next);
			continue;
		}

		maze->map[pos.row][pos.col] = 3;
		StackPop(s);
	}
}

void PassMazeNor(Maze*maze, Point entry)
{
	Stack s;
	_PassMazeNor(maze, entry, &s);
	while (StackSize(&s) > 1)
	{
		Point cur = StackTop(&s);
		printf("(%d, %d)<--", cur.row, cur.col);
		StackPop(&s);
	}
	printf("(%d, %d)\n", entry.row, entry.col);
}

/////////////////////////////////
//2.简单递归
/////////////////////////////////
int _PassMaze(Maze*maze, Point cur, Point entry)
{
	if (IsPass(maze, cur))
	{
		Point next;
		maze->map[cur.row][cur.col] = 2;
		if (IsExit(maze, cur, entry))
		{
			return;
		}

		//up
		next = cur;
		next.row -= 1;
		if (_PassMaze(maze, next, cur))
		{
			return 1;
		}
		//left
		next = cur;
		next.col -= 1;
		if (_PassMaze(maze, next, cur))
		{
			return 1;
		}
		//right
		next = cur;
		next.col += 1;
		if (_PassMaze(maze, next, cur))
		{
			return 1;
		}
		//down
		next = cur;
		next.row += 1;
		if (_PassMaze(maze, next, cur))
		{
			return 1;
		}
		maze->map[cur.row][cur.col] = 3;
	}
	return 0;
}

//void PassMaze(Maze*maze, Point entry)
//{
//	Stack s;
//	_PassMaze(&maze, entry, entry);
//	/*while (!StackEmpty(&s))
//	{
//		Point cur = StackTop(&s);
//		printf("(%d, %d)<--", cur.row, cur.col);
//		StackPop(&s);
//	}*/
//	printf("(%d, %d)\n", entry.row, entry.col);
//}

/////////////////////////////////
//最短路径
/////////////////////////////////
int IsNextPass(Maze* maze, Point cur, Point next)
{
	if (next.row >= 0 && next.row < MAX_ROW &&
		next.col >= 0 && next.col < MAX_COL)
	{
		if (1 == maze->map[next.row][next.col] ||
			maze->map[cur.row][cur.col] < maze->map[next.row][next.col])
		{
			return 1;
		}
	}
	return 0;
}
void SaveShortPath(Stack* path, Stack* short_path)
{
	int i = 0;
	for (; i < StackSize(path); ++i)
	{
		short_path->_array[i] = path->_array[i];
	}
	short_path->_size = i;
}

int IsShortExit(Point cur, Point entry)
{
	if (cur.col == entry.col && cur.row == entry.row)
		return 0;
	if (cur.row == 0 || cur.row == MAX_ROW - 1 ||
		cur.col == 0 || cur.col == MAX_COL - 1)
	{
		return 1;
	}
	return 0;
}
void _GetShortPath(Maze* maze, Point cur, Point entry, Stack* path, Stack* short_path)
{
	Point next;

	if (IsShortExit(cur, entry))
	{
		if (StackEmpty(short_path) ||
			StackSize(path) < StackSize(short_path))
		{
			SaveShortPath(path, short_path);
			return;
		}
	}
	if (StackEmpty(path))
	{
		maze->map[cur.row][cur.col] = 2;
	}

	StackPush(path, cur);
	//up
	next = cur;
	next.row -= 1;
	if (IsNextPass(maze, cur, next))
	{
		maze->map[next.row][next.col] = maze->map[cur.row][cur.col] + 1;
		_GetShortPath(maze, next, entry, path, short_path);
	}
	//left
	next = cur;
	next.col -= 1;
	if (IsNextPass(maze, cur, next))
	{
		maze->map[next.row][next.col] = maze->map[cur.row][cur.col] + 1;
		_GetShortPath(maze, next, entry, path, short_path);
	}
	//right
	next = cur;
	next.col += 1;
	if (IsNextPass(maze, cur, next))
	{
		maze->map[next.row][next.col] = maze->map[cur.row][cur.col] + 1;
		_GetShortPath(maze, next, entry, path, short_path);
	}
	//down
	next = cur;
	next.row += 1;
	if (IsNextPass(maze, cur, next))
	{
		maze->map[next.row][next.col] = maze->map[cur.row][cur.col] + 1;
		_GetShortPath(maze, next, entry, path, short_path);
	}
}
void GetShortPath(Maze* maze, Point entry)
{
	Stack path, short_path;
	if (!IsValidEntry(maze, entry))
	{
		return;
	}
	StackInit(&path);
	StackInit(&short_path);
	_GetShortPath(maze, entry, entry, &path, &short_path);
}

/////////////////////////////////
//测试
/////////////////////////////////

void TestMazeNor()
{
	TEST_HEADER;
	Point entry;
	Maze maze;
	InitMaze(&maze);
	PrintMaze(&maze);
	printf("\n");
	entry.row = 5;
	entry.col = 2;
	PassMazeNor(&maze, entry, entry);
	PrintMaze(&maze);
}
void TestMaze()
{
	TEST_HEADER;
	Point entry;
	Maze maze;
	InitMaze(&maze);
	PrintMaze(&maze);
	printf("\n");
	entry.row = 5;
	entry.col = 2;
	_PassMaze(&maze, entry, entry);
	PrintMaze(&maze);
}

void TestGetShortPath()
{
	TEST_HEADER;
	Point entry;
	Maze maze;
	InitMaze(&maze);
	PrintMaze(&maze);
	printf("\n");
	entry.row = 5;
	entry.col = 1;
	GetShortPath(&maze, entry);
	PrintMaze(&maze);
}