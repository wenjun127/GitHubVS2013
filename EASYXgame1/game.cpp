/////////////////////////////////////////////////////////
//�������ƣ�7 X 7
//���뻷����Visual studio 2012��EasyX 2013������
// 
//��   ��:
//����޸�:

/* ϵͳ�� */
#include <string.h>
#include <graphics.h>	
#include <conio.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include "HeapMinQueue.h"
#include "stack.h"
#include "game.h"


/*--------------ȫ�ֱ���-----------------*/

int clockL=0;  /*�����жϵ�һ�ε�����ǵڶ���*/
                     
int color[6]={0,0,0,0,0,0}; //����Ҫ�������ɫ��
int locaColor[6] ={0,0,0,0,0,0};

//����հ������ڵ�λ��
int loca[MAX_LINE*MAX_LINE] = {0};
int visit[MAX_LINE][MAX_LINE];  /*��Ϊ���������λ��*/

int linkColor;  

int colorNum =3;		/*Ҫ��ӵ���ɫ��*/
int combo =0;           /*combo ��*/
int score =0;			/*�÷�*/
int expre =0;			/*�����*/
int level =1;			/*�ȼ�*/


//�������λ����ɫ����ʼ��ΪWHITE
int bk[MAX_LINE][MAX_LINE]={
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
};

//ȫ�ֱ���
RECT rcTarget;
MOUSEMSG m;  /*���*/

////////////////////����������/////////////////////
void InitStack(SqStack *S);				/* ����һ����ջS */
void Push(SqStack *S,SElemType e);		/* ����Ԫ��eΪ�µ�ջ��Ԫ�� */
int Pop(SqStack *S,SElemType *e);		/* ��ջ���գ���ɾ��S��ջ��Ԫ�أ���e������ֵ��������OK�����򷵻�ERROR */
int StackEmpty(SqStack S);				/* ��ջSΪ��ջ���򷵻�TRUE�����򷵻�FALSE */
void switchSetColor(int);
int MazePath(PosType startx,PosType endx);	/* ���Թ�m�д��ڴ����start������end��ͨ���������һ�� */
											/* �����ջ��(��ջ�׵�ջ��)��������TRUE�����򷵻�FALSE */
void refresh();				//ͨ��bk��ˢ�½���
int Judgenm(int n,int m);	//�жϣ�n,m���ܷ�����
int judgeLink();			/*�ж��ܷ�����*/
void randomcolor(int putNum);/*������ɫ�����putNum����ɫ����������ͬ����ɫ*/
int randomxy(int putNum);	/*���������ɫ���������հ�������*/
void init();				//�����ʼ��




int MazePath(PosType startx,PosType endx)
 { /* ���Թ�m�д��ڴ����start������end��ͨ���������һ�� */
   /* �����ջ��(��ջ�׵�ջ��)��������TRUE�����򷵻�FALSE */
   SqStack S; /* ˳��ջ */
   PosType curpos; /* ��ǰλ�� */
   PosType end,start;
   end.x = endx.x+1;
   end.y = endx.y+1;
   start.x = startx.x+1;
   start.y = startx.y+1;
   SElemType e; /* ջԪ�� */
   InitStack(&S); /* ��ʼ��ջ */
   curpos=start; /* ��ǰλ������� */
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
     { /* ��ǰλ�ÿ���ͨ��������δ���ߵ�����ͨ���� */
       FootPrint(curpos); /* �����㼣 */
       e.ord=curstep;
       e.seat=curpos;
       e.di=0;
       Push(&S,e); /* ��ջ��ǰλ�ü�״̬ */
       curstep++; /* �㼣��1 */
       if(curpos.x==end.x&&curpos.y==end.y) /* �����յ�(����) */
         return TRUE;
       NextPos(&curpos,e.di); /* �ɵ�ǰλ�ü��ƶ�����ȷ����һ����ǰλ�� */
     }
     else
     { /* ��ǰλ�ò���ͨ�� */
       if(!StackEmpty(S)) /* ջ���� */
       {
         Pop(&S,&e); /* ��ջ��ǰһλ�� */
         curstep--; /* �㼣��1 */
         while(e.di==3&&!StackEmpty(S)) /* ǰһλ�ô������һ������(��) */
         {
           MarkPrint(e.seat); /* ��ǰһλ�����²���ͨ���ı��(-1) */
           Pop(&S,&e); /* ���˻�һ�� */
           curstep--; /* �㼣�ټ�1 */
         }
         if(e.di<3) /* û�����һ������(��) */
         {
           e.di++; /* ����һ������̽�� */
           Push(&S,e); /* ��ջ��λ�õ���һ������ */
           curstep++; /* �㼣��1 */
           curpos=e.seat; /* ȷ����ǰλ�� */
           NextPos(&curpos,e.di); /* ȷ����һ����ǰλ���Ǹ��·����ϵ����ڿ� */
         }
       }
     }
   }while(!StackEmpty(S));
   return FALSE;
}
/////////////////////////Astar�����㷨////////////////

//�����ɢ����
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
		{//0��ͬ����ƫ��
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

//ͨ��bk��ˢ�½���
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

//�жϺ����Ƿ����ĵ����� 
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


//�ж������Ƿ����ĵ����� 
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

//�ж�����б���Ƿ����ĵ�����
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

//�ж�����б���Ƿ����ĵ�����
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
{ /*�ж��ܷ�����*/
	int num=0;
	memset(visit,0,sizeof(visit)); /*��ʼ���������*/
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
		fillrectangle(360,UPLINK,370,UPLINK-INTERVAL+LINE_HEIGHT*MAX_LINE); //ˢ��
		setfillcolor(GREEN); //�����ɫ�ľ�����
		fillrectangle(360,UPLINK-INTERVAL+LINE_HEIGHT*MAX_LINE-(-INTERVAL+LINE_HEIGHT*MAX_LINE)*(expre%LEVELEXP)/LEVELEXP,
			370,UPLINK-INTERVAL+LINE_HEIGHT*MAX_LINE);
		score+=(num-4)*8+20+combo*20;  //����÷ֵĹ�ʽ
		TCHAR s[15];
		wsprintf(s,_T("SCORE:%d"),score);
		outtextxy(400,UPNEXT-80,s);
	}
	refresh();
	return (num>0);
}

/*����������ѡ����ɫ*/
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

/*������ɫ�����putNum����ɫ����������ͬ����ɫ*/
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

/*���������ɫ���������հ�������*/
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
			judgeLink(); //ÿ�������ɫ���ж�����
		}
		else return FALSE;
	}
		
	return TRUE;
}

//�����ʼ��
void init()
{
	// ������ɫ
	settextcolor(GREEN);
//	settextstyle(16,0,_T("����"));
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
	// ���������������
	srand((unsigned) time(NULL));
	PosType start,end;
//	judgeLink();

	// ��ʼ��ͼ��ģʽ
	initgraph(640, 480);
	settextstyle(16, 8, _T("Courier"));	// ��������
	SetWindowText(GetHWnd(), _T("7 X 7"));
	cleardevice();
	init();
	setlinecolor(WHITE);
	randomcolor(colorNum);
	randomxy(colorNum);

	randomcolor(colorNum);
	while(true)
	{
		// ��ȡһ�������Ϣ
		m = GetMouseMsg();
		switch(m.uMsg)
		{
			case WM_MOUSEMOVE:
				// ����ƶ���ʱ�򻭺�ɫ��С��
		//		putpixel(m.x, m.y, RED);
				break;

			case WM_LBUTTONDOWN:
				// ��������
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
						//��ǵ���ķ���
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
						{ /*������*/
							TCHAR s[10];
							combo++;
							wsprintf(s,_T("COMBO:%d"),combo);
							outtextxy(400,UPNEXT-50,s);
							break;
						}
						else{
							/*��������*/
							TCHAR s[10];
							Sleep(100);
							combo=0;
							wsprintf(s,_T("COMBO:%d"),combo);
							outtextxy(400,UPNEXT-50,s);
							if(randomxy(colorNum)==0)
							{
								
								MessageBox(NULL,_T("GAME OVER"),_T("�˳�"),MB_ICONINFORMATION|MB_YESNO);
								return 0;
							}
							randomcolor(colorNum);	
						}
						}
						else 
						{
							//δ�ܵ���ĵط�x���
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
				// ������Ҽ�
				break;
		}
	}
	// �ر�ͼ��ģʽ
	closegraph();
	
	return 0;
}
