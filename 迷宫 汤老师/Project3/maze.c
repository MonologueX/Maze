#include "Stack.h"

#define MAX_ROW 6
#define MAX_COL 6
#define TEST_HEADER printf("\n============%s============\n",__FUNCTION__)

typedef struct Maze
{
	int map[MAX_ROW][MAX_COL];
}Maze;

/////////////////////////////////
//����1
/////////////////////////////////
//��̬��ά����
//��ʼ��
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

//��ӡ
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

//�ݹ����Թ���������ջ��ջ��ϵͳ�������ߺ�������
//�ܷ��߳�


//�ж� pt ������Ƿ������
//�������ţ�����1�����򷵻�0
int CanStay(Maze* maze, Point pt)
{
	//��� pt �ڵ�ͼ�⣬�������
	if (pt.row < 0 || pt.row >= MAX_ROW ||
		pt.col < 0 || pt.col >= MAX_COL)
	{
		return 0;
	}
	//��� pt �ڵ�ͼ�ڣ�pt Ϊ1����ţ�pt Ϊ0����2���������
	int value = maze->map[pt.row][pt.col];
	if (value == 1)
	{
		return 1;
	}
	return 0;
}

//���
void Mark(Maze* maze, Point cur)
{
	maze->map[cur.row][cur.col] = 2;
}

//����ǳ��ڣ�����1�����򷵻�0
int IsExit(Maze* maze, Point cur, Point entry)
{
	(void)maze;//�������
	//��ǰ���ǲ�����ڣ�����ڣ����ǳ���
	if (cur.row == entry.row && cur.col == entry.col)
	{
		return 0;
	}
	//����ڵ�ͼ�ı߽��ϣ�Ҳ˵���ǳ���
	if (cur.row == 0 || cur.row == MAX_ROW - 1 ||
		cur.col == 0 || cur.col == MAX_COL - 1)
	{
		return 1;
	}
	return 0;
}
//ÿ���ߵ���һ���㣬����ݹ�������º���
void _GetPath(Maze* maze, Point cur, Point entry)
{
	printf("cur:%d %d\n", cur.row, cur.col);
	//1.�ж���ǰ���Ƿ�����
	if (!CanStay(maze, cur))
	{
		return;
	}
	//2.�������ţ�����ǰλ�������
	Mark(maze, cur);
	//3.�����ǰ���ǳ��ڣ�˵���ҵ���һ����·��̽�����
	if (IsExit(maze, cur, entry))
	{
		printf("�ҵ�һ��·��\n");
		return;
	}
	//4.�����ǰ�㲻�ǳ��ڣ�˳ʱ��̽���ĸ����ڵ�
	//  �ݹ���ú��������ݹ�ʱ������cur�㣨ÿ�εݹ�ʱ��
	//  �㶼����һ��Ҫ�ߵĵ㣬������ܷ��߽����ݹ麯���ж�)
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
	//ʹ�����溯�������ݹ�
	_GetPath(maze, entry, entry);
}

/////////////////////////////////
//����2 �ǵݹ�
/////////////////////////////////

void GetPathByLoop(Maze* maze, Point entry)
{
	//1.����һ��ջ�����ҳ�ʼ�������ջ�������߹���·��
	Stack stack;
	StackInit(&stack);
	//2.�ж�����ܲ�����ţ�������ܣ�˵�������Ƿ�
	if (!CanStay(maze, entry))
	{
		return;
	}
	//3.�����ڵ㣬���ҽ���ڵ���ջ
	Mark(maze, entry);
	StackPush(&stack, entry);
	while (1)
	{
		//4.����ѭ������ȡ����ǰ��ջ��Ԫ�أ�ջ��Ԫ��һ������ţ�
		Point cur;
		int ret = StackTop(&stack, &cur);
		if (ret == 0)
		{
			//ջΪ��˵�����ݽ���
			return;
		}
		//5.�ж�������ǲ��ǳ��ڣ�����ǳ��ڣ�ֱ�Ӻ�������
		if (IsExit(maze, cur, entry))
		{
			printf("�ҵ�һ��·��\n");
			return;
		}
		//6.����˳ʱ�뷽��ȡ���ڵ㣬�ж����ڵ��ܷ���ţ�
		//  �������ţ��������ջ�����̽�����һ��ѭ��
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
		//7.����ĸ����ڵ㶼������ţ��ͳ�ջ��ǰ�㣬�൱�ڽ��л���
		StackPop(&stack);
	}
	return;
}

/////////////////////////////////
//����2 ���
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
//	//1.����һ��ջ�����ҳ�ʼ�������ջ�������߹���·��
//	StackInit(&cur_path);
//	StackInit(&short_path);
//	_GetShortPath(maze, entry, &cur_path, &short_path);
//
//}

//////////////////////////////////
//���Դ���
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