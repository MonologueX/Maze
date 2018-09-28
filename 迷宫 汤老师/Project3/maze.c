#include "Stack.h"

#define MAX_ROW 6
#define MAX_COL 6
#define TEST_HEADER printf("\n============%s============\n",__FUNCTION__)

typedef struct Maze
{
	int map[MAX_ROW][MAX_COL];
}Maze;

/////////////////////////////////
//函数1
/////////////////////////////////
//动态二维数组
//初始化
void MazeInit(Maze* maze)
{
	int map[MAX_ROW][MAX_COL] = {
		{ 0, 1, 0, 0, 0, 0 },
		{ 0, 1, 1, 1, 0, 0 },
		{ 0, 1, 0, 1, 0, 0 },
		{ 0, 1, 0, 1, 1, 0 },
		{ 0, 1, 1, 0, 0, 0 },
		{ 0, 0, 1, 0, 0, 0 },
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

//打印
void MazePrint(Maze* maze)
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

//递归解决迷宫，本质是栈，栈是系统给，或者函数调用
//能否走出


//判定 pt 这个点是否能落脚
//如果能落脚，返回1，否则返回0
int CanStay(Maze* maze, Point pt)
{
	//如果 pt 在地图外，不能落脚
	if (pt.row < 0 || pt.row >= MAX_ROW ||
		pt.col < 0 || pt.col >= MAX_COL)
	{
		return 0;
	}
	//如果 pt 在地图内，pt 为1可落脚，pt 为0或者2，不能落脚
	int value = maze->map[pt.row][pt.col];
	if (value == 1)
	{
		return 1;
	}
	return 0;
}

//标记
void Mark(Maze* maze, Point cur)
{
	maze->map[cur.row][cur.col] = 2;
}

//如果是出口，返回1，否则返回0
int IsExit(Maze* maze, Point cur, Point entry)
{
	(void)maze;//清除警告
	//当前点是不是入口，是入口，不是出口
	if (cur.row == entry.row && cur.col == entry.col)
	{
		return 0;
	}
	//如果在地图的边界上，也说明是出口
	if (cur.row == 0 || cur.row == MAX_ROW - 1 ||
		cur.col == 0 || cur.col == MAX_COL - 1)
	{
		return 1;
	}
	return 0;
}
//每次走到下一个点，都会递归调用以下函数
void _GetPath(Maze* maze, Point cur, Point entry)
{
	printf("cur:%d %d\n", cur.row, cur.col);
	//1.判定当前点是否能走
	if (!CanStay(maze, cur))
	{
		return;
	}
	//2.如果能落脚，给当前位置做标记
	Mark(maze, cur);
	//3.如果当前点是出口，说明找到了一条出路，探测结束
	if (IsExit(maze, cur, entry))
	{
		printf("找到一条路径\n");
		return;
	}
	//4.如果当前点不是出口，顺时针探测四个相邻点
	//  递归调用函数自身，递归时，更新cur点（每次递归时，
	//  点都是下一次要走的点，这个点能否走交给递归函数判断)
	Point up = cur;
	up.row -= 1;
	_GetPath(maze, up, entry);

	Point right = cur;
	right.col += 1;
	_GetPath(maze, right, entry);

	Point down = cur;
	down.row += 1;
	_GetPath(maze, down, entry);

	Point left = cur;
	left.col -= 1;
	_GetPath(maze, left, entry);
}

void GetPath1(Maze* maze, Point entry)
{
	if (maze == NULL)
	{
		return;
	}
	//使用下面函数辅助递归
	_GetPath(maze, entry, entry);
}

/////////////////////////////////
//函数2 非递归
/////////////////////////////////

void GetPathByLoop(Maze* maze, Point entry)
{
	//1.创建一个栈，并且初始化，这个栈保存着走过的路径
	Stack stack;
	StackInit(&stack);
	//2.判定入口能不能落脚，如果不能，说明参数非法
	if (!CanStay(maze, entry))
	{
		return;
	}
	//3.标记入口点，并且将入口点入栈
	Mark(maze, entry);
	StackPush(&stack, entry);
	while (1)
	{
		//4.进入循环，获取到当前的栈顶元素（栈顶元素一定能落脚）
		Point cur;
		int ret = StackTop(&stack, &cur);
		if (ret == 0)
		{
			//栈为空说明回溯结束
			return;
		}
		//5.判断这个点是不是出口，如果是出口，直接函数返回
		if (IsExit(maze, cur, entry))
		{
			printf("找到一条路径\n");
			return;
		}
		//6.按照顺时针方向取相邻点，判定相邻点能否落脚，
		//  如果能落脚，标记且入栈，立刻进入下一轮循环
		Point up = cur;
		up.row -= 1;
		if (CanStay(maze, up))
		{
			Mark(maze, up);
			StackPush(&stack,up);
			continue;
		}
		Point right = cur;
		up.col += 1;
		if (CanStay(maze, right))
		{
			Mark(maze, right);
			StackPush(&stack, right);
			continue;
		}
		Point down = cur;
		up.row += 1;
		if (CanStay(maze, down))
		{
			Mark(maze, down);
			StackPush(&stack, down);
			continue;
		}
		Point left = cur;
		up.col -= 1;
		if (CanStay(maze, left))
		{
			Mark(maze, left);
			StackPush(&stack, left);
			continue;
		}
		//7.如果四个相邻点都不能落脚，就出栈当前点，相当于进行回溯
		StackPop(&stack);
	}
	return;
}

/////////////////////////////////
//函数2 最短
/////////////////////////////////

//void MazeInitShortPath(Maze* maze)
//{
//	int map[MAX_ROW][MAX_COL] = {
//		{ 0, 1, 0, 0, 0, 0 },
//		{ 0, 1, 1, 1, 0, 0 },
//		{ 0, 1, 0, 1, 1, 1 },
//		{ 1, 1, 1, 0, 0, 0 },
//		{ 0, 0, 1, 0, 0, 0 },
//		{ 0, 0, 1, 0, 0, 0 },
//	};
//	size_t i = 0;
//	for (; i < MAX_ROW; ++i)
//	{
//		size_t j = 0;
//		for (; j < MAX_COL; ++j)
//		{
//			maze->map[i][j] = map[i][j];
//		}
//	}
//	return;
//}
//
//void StackDebugPrint(Stack* stack, const char* msg)
//{
//	printf("[%s]\n", msg);
//	size_t i = 0;
//	for (; i < stack->_size; ++i)
//	{
//		printf("(%d,%d)\n", stack->_array[i].row, stack->_array[i].row);
//	}
//	printf("\n");
//}
//
//void _GetShortPath(Maze* maze, Point entry)
//{
//
//}
//
//void GetShortPath(Maze* maze, Point entry)
//{
//	Stack cur_path;
//	Stack short_path;
//	//1.创建一个栈，并且初始化，这个栈保存着走过的路径
//	StackInit(&cur_path);
//	StackInit(&short_path);
//	_GetShortPath(maze, entry, &cur_path, &short_path);
//
//}

//////////////////////////////////
//测试代码
/////////////////////////////////

void TestInit()
{
	TEST_HEADER;
	Maze maze;
	MazeInit(&maze);
	MazePrint(&maze);
}

void Test1()
{
	TEST_HEADER;
	Maze maze;
	MazeInit(&maze);
	Point entry = { 0, 1 };
	GetPath1(&maze, entry);
	/*GetPathByLoop(&maze, entry);*/
	MazePrint(&maze);
}

int main()
{
	TestInit();
	Test1();
	system("pause");
	return 0;
}