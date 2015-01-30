#include "game.h"

#define STACK_INIT_SIZE 10	/* �洢�ռ��ʼ������ */
#define STACK_INCREMENT 2	/* �洢�ռ�������� */

///////////////�õ��Ľṹ��////////////////
typedef struct
{
	int x;	/* ��ֵ */
	int y;	/* ��ֵ */
}PosType;	/* �Թ�����λ������ */

typedef struct
{
	int ord;		/* ͨ������·���ϵģ���ţ� */
	PosType seat;	/* ͨ�������Թ��еģ�����λ�ã� */
	int di;			/* �Ӵ�ͨ����������һͨ����ģ�����(0��3��ʾ������) */
}SElemType;	/* ջ��Ԫ������ */

typedef struct SqStack
{
	SElemType *base;	/* ��ջ����֮ǰ������֮��base��ֵΪNULL */
  SElemType *top;		/* ջ��ָ�� */
  int stacksize;		/* ��ǰ�ѷ���Ĵ洢�ռ䣬��Ԫ��Ϊ��λ */
}SqStack;		/* ˳��ջ */

typedef int MazeType[MAX_LINE+2][MAX_LINE+2]; /* �Թ���������[��][��] */


/*--------------ȫ�ֱ���-----------------*/

MazeType maze;  /* �Թ����� */
int curstep =1; 

//////////////////////////ջ����/////////////////////////
void InitStack(SqStack *S)
{ /* ����һ����ջS */
	(*S).base=(SElemType *)malloc(STACK_INIT_SIZE*sizeof(SElemType));
	if(!(*S).base)
		exit(0);	/* �洢����ʧ�� */
	(*S).top=(*S).base;
	(*S).stacksize=STACK_INIT_SIZE;
}

void Push(SqStack *S,SElemType e)
{ /* ����Ԫ��eΪ�µ�ջ��Ԫ�� */
	if((*S).top-(*S).base>=(*S).stacksize)	/* ջ����׷�Ӵ洢�ռ� */
	{
		(*S).base=(SElemType *)realloc((*S).base,((*S).stacksize+STACK_INCREMENT)*sizeof(SElemType));
		if(!(*S).base)
			exit(0);	/* �洢����ʧ�� */
		(*S).top=(*S).base+(*S).stacksize;
		(*S).stacksize+=STACK_INCREMENT;
	}
	*((*S).top)++=e;
}

int Pop(SqStack *S,SElemType *e)
{	/* ��ջ���գ���ɾ��S��ջ��Ԫ�أ���e������ֵ��������OK�����򷵻�ERROR */
	if((*S).top==(*S).base)
		return ERROR;
	*e=*--(*S).top;
	return OK;
}

int StackEmpty(SqStack S)
{ /* ��ջSΪ��ջ���򷵻�TRUE�����򷵻�FALSE */
	if(S.top==S.base)
		return TRUE;
	else
		return FALSE;
}

//////////////�Թ�����////////////////
int Pass(PosType b)
 { /* ���Թ�m��b������Ϊ1(��ͨ��·��)������OK�����򣬷���ERROR */
   if(maze[b.x][b.y]==1)
     return OK;
   else
     return ERROR;
 }

void FootPrint(PosType a)
 { /* ʹ�Թ�m��a���ֵ��Ϊ�㼣(curstep) */
   maze[a.x][a.y]=curstep;
 }

void NextPos(PosType *c,int di)
 { /* ���ݵ�ǰλ�ü��ƶ����������һλ�� */
   PosType direc[4]={{0,1},{1,0},{0,-1},{-1,0}}; /* {������,������},�ƶ�����,����Ϊ�������� */
   (*c).x+=direc[di].x;
   (*c).y+=direc[di].y;
 }

void MarkPrint(PosType b)
 { /* ʹ�Թ�m��b�����ű�Ϊ-1(����ͨ����·��) */
   maze[b.x][b.y]=-1;
 }