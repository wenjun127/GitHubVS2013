/////////////////////////////////////////////////////////
//程序名称：7 X 7
//编译环境：Visual studio 2012，EasyX 2013冬至版
// 
//作   者:
//最后修改:

/* 系统库 */
#include <string.h>
#include <graphics.h>	
#include <conio.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include "HeapMinQueue.h"
#include "stack.h"
#include "game.h"


/*--------------全局变量-----------------*/

int clockL=0;  /*锁存判断第一次点击还是第二次*/
                     
int color[6]={0,0,0,0,0,0}; //保存要加入的颜色块
int locaColor[6] ={0,0,0,0,0,0};

//保存空白区所在的位置
int loca[MAX_LINE*MAX_LINE] = {0};
int visit[MAX_LINE][MAX_LINE];  /*作为标记消除的位置*/

int linkColor;  

int colorNum =3;		/*要添加的颜色数*/
int combo =0;           /*combo 数*/
int score =0;			/*得分*/
int expre =0;			/*经验分*/
int level =1;			/*等级*/


//保存各个位置颜色，初始均为WHITE
int bk[MAX_LINE][MAX_LINE]={
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
};

//全局变量
RECT rcTarget;
MOUSEMSG m;  /*鼠标*/

////////////////////函数的申明/////////////////////
void InitStack(SqStack *S);				/* 构造一个空栈S */
void Push(SqStack *S,SElemType e);		/* 插入元素e为新的栈顶元素 */
int Pop(SqStack *S,SElemType *e);		/* 若栈不空，则删除S的栈顶元素，用e返回其值，并返回OK；否则返回ERROR */
int StackEmpty(SqStack S);				/* 若栈S为空栈，则返回TRUE，否则返回FALSE */
void switchSetColor(int);
int MazePath(PosType startx,PosType endx);	/* 若迷宫m中存在从入口start到出口end的通道，则求得一条 */
											/* 存放在栈中(从栈底到栈顶)，并返回TRUE；否则返回FALSE */
void refresh();				//通过bk来刷新界面
int Judgenm(int n,int m);	//判断（n,m）能否消除
int judgeLink();			/*判断能否消除*/
void randomcolor(int putNum);/*从种颜色中随机putNum个颜色，可以是相同的颜色*/
int randomxy(int putNum);	/*将随机的颜色随机的填入空白网格中*/
void init();				//界面初始化




int MazePath(PosType startx,PosType endx)
 { /* 若迷宫m中存在从入口start到出口end的通道，则求得一条 */
   /* 存放在栈中(从栈底到栈顶)，并返回TRUE；否则返回FALSE */
   SqStack S; /* 顺序栈 */
   PosType curpos; /* 当前位置 */
   PosType end,start;
   end.x = endx.x+1;
   end.y = endx.y+1;
   start.x = startx.x+1;
   start.y = startx.y+1;
   SElemType e; /* 栈元素 */
   InitStack(&S); /* 初始化栈 */
   curpos=start; /* 当前位置在入口 */
   for(int i = 0;i <MAX_LINE+2;i++)
	{
		for(int j = 0;j < MAX_LINE+2;j ++)
		{
			if(i==0||i==MAX_LINE+1||j==0||j==MAX_LINE+1)
				maze[i][j] = 0;
			else{
			if(bk[i-1][j-1]==0)
				maze[i][j]=1;
			else
				maze[i][j]=0;
			}
		}
   }
   maze[start.x][start.y]=1;

   do
   {
     if(Pass(curpos))
     { /* 当前位置可以通过，即是未曾走到过的通道块 */
       FootPrint(curpos); /* 留下足迹 */
       e.ord=curstep;
       e.seat=curpos;
       e.di=0;
       Push(&S,e); /* 入栈当前位置及状态 */
       curstep++; /* 足迹加1 */
       if(curpos.x==end.x&&curpos.y==end.y) /* 到达终点(出口) */
         return TRUE;
       NextPos(&curpos,e.di); /* 由当前位置及移动方向，确定下一个当前位置 */
     }
     else
     { /* 当前位置不能通过 */
       if(!StackEmpty(S)) /* 栈不空 */
       {
         Pop(&S,&e); /* 退栈到前一位置 */
         curstep--; /* 足迹减1 */
         while(e.di==3&&!StackEmpty(S)) /* 前一位置处于最后一个方向(北) */
         {
           MarkPrint(e.seat); /* 在前一位置留下不能通过的标记(-1) */
           Pop(&S,&e); /* 再退回一步 */
           curstep--; /* 足迹再减1 */
         }
         if(e.di<3) /* 没到最后一个方向(北) */
         {
           e.di++; /* 换下一个方向探索 */
           Push(&S,e); /* 入栈该位置的下一个方向 */
           curstep++; /* 足迹加1 */
           curpos=e.seat; /* 确定当前位置 */
           NextPos(&curpos,e.di); /* 确定下一个当前位置是该新方向上的相邻块 */
         }
       }
     }
   }while(!StackEmpty(S));
   return FALSE;
}
/////////////////////////Astar搜索算法////////////////

//计算耗散函数
int calH(OPEN source,PosType endx)
{
	int s=(endx.x-source.x)*(endx.x-source.x)+(source.y-endx.y)*(source.y-endx.y);
	int h =(int)sqrt(double(s));
	return h;
}

int move(OPEN head,int i)
{
	int x=head.x,y=head.y;
	x+=xtran[i];
	y+=ytran[i];

	if((x<0 || x>=MAX_LINE || y<0 || y>+MAX_LINE))
	{
		return -1;
	}
	if(bk[x][y]!=0)
		return -2;
	return x*MAX_LINE+y;
}

int AstarPath(PosType startx,PosType endx)
{
	memset(flag,0,sizeof(flag));
	heap_size=closeSize =0;
	OPEN source;
	source.x = startx.x;
	source.y = startx.y;
	source.g = 0;
	source.h = calH(source,endx);
	source.f=source.g+source.h;
	source.pre = -1;
	visit[source.x][source.y]=1;

	addMinHeap(open,source);
	while(heap_size>0)
	{
		OPEN head=open[0];
		close[closeSize++] =head;
		HeapExtractMin(open);
		if((head.x*MAX_LINE+head.y) == (endx.x*MAX_LINE+endx.y))
		{
			printf("find the end point!\n");
			return TRUE;
		}
		for(int i =0;i <4;i++)
		{//0向不同方向偏移
			int local = move(head,i);
			
			if(local>=0)
			{
				int x=local/MAX_LINE;
				int y = local%MAX_LINE;
				OPEN next;
				next.x = x;
				next.y = y;
				next.g = head.g+1;
				next.h = calH(next,endx);
				next.f = next.g+next.h;
				next.pre = closeSize-1;

				if(visit[next.x][next.y]==0)
					addMinHeap(open,next);
				visit[next.x][next.y]=1;

			}
		}

	}
	return FALSE;
}
//////////////////////////////////////////////////////////////

//通过bk来刷新界面
void refresh()
{
	for(int i = 0;i <MAX_LINE;i++)
	{
		for(int j = 0;j < MAX_LINE;j ++)
		{
			switchSetColor(bk[i][j]);

			rcTarget.left = UPLINK+LINE_WIDTH*j;
			rcTarget.top = UPLINK+LINE_HEIGHT*i;
			rcTarget.right = UPLINK-INTERVAL+LINE_WIDTH*(j+1);
			rcTarget.bottom = UPLINK-INTERVAL+LINE_HEIGHT*(i+1);
			fillrectangle(rcTarget.left,rcTarget.top,
				rcTarget.right,rcTarget.bottom);
		}
	}
}

int Judgenm(int n,int m)
{
	int color=bk[n][m];
	int a,b,c,d;
	a=b=c=d=0;
	int i,j,k,s,o,p;
	if(color == 0)
		return -1;

//判断横线是否有四点连线 
	for(i = m+1;i<MAX_LINE && i>=0;i++)
	{
		if(bk[n][i] == color) 
			a++;
		else  break;
	}
	for(k =m;k<MAX_LINE && k>=0;k--)
	{
		if(bk[n][k] == color) a++;
		else break;
	}
	if(a>=MAX)
	{
		for(j = m+1;j<i;j++)
			visit[n][j]=color;

		for(s = m;s>k;s--)
			visit[n][s]=color;
	}


//判断竖线是否有四点连线 
	for(i = n+1;i<MAX_LINE && i>=0;i++)
	{
		if(bk[i][m]==color) b++;
		else  break;
	}
	for(k = n;k<MAX_LINE && k>=0;k--)
	{
		if(bk[k][m] ==color) b++;
		else break;
	}
	if(b>=MAX)
	{
		for(j = n+1;j<i;j++)
			visit[j][m]  =color;
		for(s = n;s >k;s--)
			visit[s][m] = color;
	}

//判断右下斜线是否有四点连线
	for(i = n+1,j = m+1;(i<MAX_LINE&&i>=0) && (j>=0&&j<MAX_LINE);i++,j++)
	{
		if(bk[i][j]==color) c++;
		else break;
	}
	for( o =n,p = m;(o<MAX_LINE&&o>=0) && (p>=0&&p<MAX_LINE);o--,p--)
	{
		if(bk[o][p]==color) c++;
		else break;
	}
	if(c>=MAX)
	{
		for(k = n+1,s=m+1;k<i && s<j;k++,s++)
			visit[k][s]=color;

		for(k = n,s=m;k>o && s>p;k--,s--)
			visit[k][s]=color;
	}

//判断左下斜线是否有四点连线
	for(i = n-1,j = m+1;(i<MAX_LINE&&i>=0) && (j>=0&&j<MAX_LINE);i--,j++)
	{
		if(bk[i][j]==color) d++;
		else break;
	}
	for(o = n,p=m;(o<MAX_LINE&&o>=0) && (p>=0&&p<MAX_LINE);o++,p--)
	{
		if(bk[o][p] ==color) d++;
		else break;
	}
	if(d>=MAX)
	{
		for(k = n-1,s=m+1;k>i && s<j;k--,s++)
			visit[k][s]=color;
		for(k = n,s=m;k<o && s>p;k++,s--)
			visit[k][s] = color;
	}
	return (a>=MAX) + (b>=MAX) + (c>=MAX) + (d>=MAX);

}

int judgeLink()
{ /*判断能否消除*/
	int num=0;
	memset(visit,0,sizeof(visit)); /*初始化标记数组*/
	for(int i = 0;i <MAX_LINE;i++)
		for(int j = 0;j < MAX_LINE;j++)
		{
			if(bk[i][j]!=0)
			Judgenm(i,j);
		}
	for(int i = 0;i <MAX_LINE;i++)
		for(int j = 0;j < MAX_LINE;j++)
		{
			if(visit[i][j] !=0)
			{
				num++;
				bk[i][j] =0;
			}
		}
	
	if(num>0)
	{
		expre++;
		if(expre>=40 && level<4)
		{
			TCHAR s[10]; 
			level++;
			wsprintf(s,_T("LEVEL %d"),level);
			outtextxy(400,280,s);
			colorNum++;
			expre = expre%LEVELEXP;
		}
		setfillcolor(WHITE);
		fillrectangle(360,UPLINK,370,UPLINK-INTERVAL+LINE_HEIGHT*MAX_LINE); //刷白
		setfillcolor(GREEN); //填充绿色的经验条
		fillrectangle(360,UPLINK-INTERVAL+LINE_HEIGHT*MAX_LINE-(-INTERVAL+LINE_HEIGHT*MAX_LINE)*(expre%LEVELEXP)/LEVELEXP,
			370,UPLINK-INTERVAL+LINE_HEIGHT*MAX_LINE);
		score+=(num-4)*8+20+combo*20;  //计算得分的公式
		TCHAR s[15];
		wsprintf(s,_T("SCORE:%d"),score);
		outtextxy(400,UPNEXT-80,s);
	}
	refresh();
	return (num>0);
}

/*根据数字来选择颜色*/
void switchSetColor(int s)
{
	switch(s)
	{
	case 0:
		setfillcolor(WHITE);
		break;
	case GBLUE:
		setfillcolor(BLUE);
		break;
	case GGREEN:
		setfillcolor(GREEN);
		break;
	case GYELLOW:
		setfillcolor(YELLOW);
		break;
	case GRED:
		setfillcolor(RED);
		break;
	case GMAGENTA:
		setfillcolor(MAGENTA);
		break;
	}
}

/*从种颜色中随机putNum个颜色，可以是相同的颜色*/
void randomcolor(int putNum)
{
	for(int i=0;i<putNum;i++)
	{
		color[i] = rand()%5+1;
		switchSetColor(color[i]);
		fillrectangle(400+LINE_WIDTH*(i%3),UPNEXT+LINE_HEIGHT*(i/3),
			400-INTERVAL+LINE_WIDTH*(i%3+1),UPNEXT-INTERVAL+LINE_HEIGHT*(i/3+1));
	}
}

int numofbk()
{
	int num=0;
	for(int i =0;i <MAX_LINE;i++)
		for(int j =0;j < MAX_LINE;j++)
		{
			if(bk[i][j]==0)
			{
				num++;
			}
		}
	return num;
}

/*将随机的颜色随机的填入空白网格中*/
int randomxy(int putNum)
{
	int i,w,h;
	for(i = 0;i< putNum;i++)
	{
		if(numofbk()>0)
		{
			do{
			int ran = rand()%(MAX_LINE*MAX_LINE);
			w = ran/MAX_LINE;
			h = ran%MAX_LINE;
			}while(bk[w][h]!=0);
			bk[w][h] = color[i];
			switchSetColor(color[i]);
			fillrectangle(UPLINK+LINE_WIDTH*(h),UPLINK+LINE_HEIGHT*(w),
				UPLINK-INTERVAL+LINE_WIDTH*(h+1),UPLINK-INTERVAL+LINE_HEIGHT*(w+1));
			Sleep(200);
			judgeLink(); //每次随机颜色都判断消除
		}
		else return FALSE;
	}
		
	return TRUE;
}

//界面初始化
void init()
{
	// 设置颜色
	settextcolor(GREEN);
//	settextstyle(16,0,_T("宋体"));
	refresh();
	TCHAR s[10];
	wsprintf(s,_T("SCORE:%d"),score);
	outtextxy(400,UPNEXT-80,s);
	wsprintf(s,_T("COMBO:%d"),combo);
	outtextxy(400,UPNEXT-50,s);

	outtextxy(400,UPNEXT-20,_T("UP NEXT"));
	outtextxy(400,280,_T("LEVEL 1"));
	outtextxy(20,330,_T("     The goal is to get the highest score by "));
	outtextxy(20,350,_T("clearing rows of blocks and utilizing combos."));
	outtextxy(20,370,_T("     To clear blocks,simple drag them to match 4 "));
	outtextxy(20,390,_T("or more of the same color blocks in a row.You can"));
	outtextxy(20,410,_T("match vertically horizontally or diagonally."));
	outtextxy(20,430,_T("     New pieces will be generated on the board "));
	outtextxy(20,450,_T("after every move."));
	outtextxy(150,310,_T("HOW TO PLAY"));
	for(int i=0;i<2;i++)
		for(int j =0;j<3;j++)
			fillrectangle(400+LINE_WIDTH*j,UPNEXT+LINE_HEIGHT*i,
			400-INTERVAL+LINE_WIDTH*(j+1),UPNEXT-INTERVAL+LINE_HEIGHT*(i+1));
	fillrectangle(360,UPLINK,370,UPLINK-INTERVAL+LINE_HEIGHT*MAX_LINE);

}

int main()
{
	// 设置随机函数种子
	srand((unsigned) time(NULL));
	PosType start,end;
//	judgeLink();

	// 初始化图形模式
	initgraph(640, 480);
	settextstyle(16, 8, _T("Courier"));	// 设置字体
	SetWindowText(GetHWnd(), _T("7 X 7"));
	cleardevice();
	init();
	setlinecolor(WHITE);
	randomcolor(colorNum);
	randomxy(colorNum);

	randomcolor(colorNum);
	while(true)
	{
		// 获取一条鼠标消息
		m = GetMouseMsg();
		switch(m.uMsg)
		{
			case WM_MOUSEMOVE:
				// 鼠标移动的时候画红色的小点
		//		putpixel(m.x, m.y, RED);
				break;

			case WM_LBUTTONDOWN:
				// 如果点左键
				int w,h;
				w = (m.x-UPLINK)/(LINE_WIDTH);
				h = (m.y-UPLINK)/(LINE_HEIGHT);
				settextcolor(GREEN);
				
				if(w<7 &&h<7)
				{
					if(bk[h][w] !=0)
					{
						start.x = h;
						start.y = w;
						clockL=1;	
						refresh();
						setfillcolor(WHITE);
						//标记点击的方块
						rectangle(UPLINK+LINE_WIDTH*(w)+10,UPLINK+LINE_HEIGHT*(h)+10,
			UPLINK-INTERVAL+LINE_WIDTH*(w+1)-10,UPLINK-INTERVAL+LINE_HEIGHT*(h+1)-10);

					}
					else if(clockL==1 && bk[h][w] ==0)
					{
						end.x = h;
						end.y = w;
						clockL =0;

						int yes =MazePath(start,end);

						if(yes==TRUE)
						{
							bk[end.x][end.y] = bk[start.x][start.y];
							bk[start.x][start.y] =0;
						//	refresh();
						switchSetColor(bk[end.x][end.y]);
						fillrectangle(UPLINK+LINE_WIDTH*(end.y),UPLINK+LINE_HEIGHT*(end.x),
			UPLINK-INTERVAL+LINE_WIDTH*(end.y+1),UPLINK-INTERVAL+LINE_HEIGHT*(end.x+1));
	
						setfillcolor(WHITE);
						fillrectangle(UPLINK+LINE_WIDTH*(start.y),UPLINK+LINE_HEIGHT*(start.x),
			UPLINK-INTERVAL+LINE_WIDTH*(start.y+1),UPLINK-INTERVAL+LINE_HEIGHT*(start.x+1));
						Sleep(100);
						if(judgeLink()!=0)
						{ /*能消除*/
							TCHAR s[10];
							combo++;
							wsprintf(s,_T("COMBO:%d"),combo);
							outtextxy(400,UPNEXT-50,s);
							break;
						}
						else{
							/*不能消除*/
							TCHAR s[10];
							Sleep(100);
							combo=0;
							wsprintf(s,_T("COMBO:%d"),combo);
							outtextxy(400,UPNEXT-50,s);
							if(randomxy(colorNum)==0)
							{
								
								MessageBox(NULL,_T("GAME OVER"),_T("退出"),MB_ICONINFORMATION|MB_YESNO);
								return 0;
							}
							randomcolor(colorNum);	
						}
						}
						else 
						{
							//未能到达的地方x标记
							setcolor(BLACK);
							line(UPLINK+LINE_WIDTH*(end.y),UPLINK+LINE_HEIGHT*(end.x),
						UPLINK-INTERVAL+LINE_WIDTH*(end.y+1),UPLINK-INTERVAL+LINE_HEIGHT*(end.x+1));

							line(UPLINK+LINE_WIDTH*(end.y),UPLINK-INTERVAL+LINE_HEIGHT*(end.x+1),
						UPLINK-INTERVAL+LINE_WIDTH*(end.y+1),UPLINK+LINE_HEIGHT*(end.x));
							setcolor(WHITE);
						}
					}			
				}
				break;

			case WM_RBUTTONUP:
				// 按鼠标右键
				break;
		}
	}
	// 关闭图形模式
	closegraph();
	
	return 0;
}
