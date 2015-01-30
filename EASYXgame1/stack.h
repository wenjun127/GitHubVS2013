#include "game.h"

#define STACK_INIT_SIZE 10	/* 存储空间初始分配量 */
#define STACK_INCREMENT 2	/* 存储空间分配增量 */

///////////////用到的结构体////////////////
typedef struct
{
	int x;	/* 行值 */
	int y;	/* 列值 */
}PosType;	/* 迷宫坐标位置类型 */

typedef struct
{
	int ord;		/* 通道块在路径上的＂序号＂ */
	PosType seat;	/* 通道块在迷宫中的＂坐标位置＂ */
	int di;			/* 从此通道块走向下一通道块的＂方向＂(0～3表示东～北) */
}SElemType;	/* 栈的元素类型 */

typedef struct SqStack
{
	SElemType *base;	/* 在栈构造之前和销毁之后，base的值为NULL */
  SElemType *top;		/* 栈顶指针 */
  int stacksize;		/* 当前已分配的存储空间，以元素为单位 */
}SqStack;		/* 顺序栈 */

typedef int MazeType[MAX_LINE+2][MAX_LINE+2]; /* 迷宫数组类型[行][列] */


/*--------------全局变量-----------------*/

MazeType maze;  /* 迷宫数组 */
int curstep =1; 

//////////////////////////栈操作/////////////////////////
void InitStack(SqStack *S)
{ /* 构造一个空栈S */
	(*S).base=(SElemType *)malloc(STACK_INIT_SIZE*sizeof(SElemType));
	if(!(*S).base)
		exit(0);	/* 存储分配失败 */
	(*S).top=(*S).base;
	(*S).stacksize=STACK_INIT_SIZE;
}

void Push(SqStack *S,SElemType e)
{ /* 插入元素e为新的栈顶元素 */
	if((*S).top-(*S).base>=(*S).stacksize)	/* 栈满，追加存储空间 */
	{
		(*S).base=(SElemType *)realloc((*S).base,((*S).stacksize+STACK_INCREMENT)*sizeof(SElemType));
		if(!(*S).base)
			exit(0);	/* 存储分配失败 */
		(*S).top=(*S).base+(*S).stacksize;
		(*S).stacksize+=STACK_INCREMENT;
	}
	*((*S).top)++=e;
}

int Pop(SqStack *S,SElemType *e)
{	/* 若栈不空，则删除S的栈顶元素，用e返回其值，并返回OK；否则返回ERROR */
	if((*S).top==(*S).base)
		return ERROR;
	*e=*--(*S).top;
	return OK;
}

int StackEmpty(SqStack S)
{ /* 若栈S为空栈，则返回TRUE，否则返回FALSE */
	if(S.top==S.base)
		return TRUE;
	else
		return FALSE;
}

//////////////迷宫操作////////////////
int Pass(PosType b)
 { /* 当迷宫m的b点的序号为1(可通过路径)，返回OK；否则，返回ERROR */
   if(maze[b.x][b.y]==1)
     return OK;
   else
     return ERROR;
 }

void FootPrint(PosType a)
 { /* 使迷宫m的a点的值变为足迹(curstep) */
   maze[a.x][a.y]=curstep;
 }

void NextPos(PosType *c,int di)
 { /* 根据当前位置及移动方向，求得下一位置 */
   PosType direc[4]={{0,1},{1,0},{0,-1},{-1,0}}; /* {行增量,列增量},移动方向,依次为东南西北 */
   (*c).x+=direc[di].x;
   (*c).y+=direc[di].y;
 }

void MarkPrint(PosType b)
 { /* 使迷宫m的b点的序号变为-1(不能通过的路径) */
   maze[b.x][b.y]=-1;
 }