#ifndef ASTAR_H
#define ASTER_H

typedef struct tagOPEN
{
	int x,y;
	int h;
	int g;
	int f;
	int pre;
}OPEN;

OPEN open[49];
OPEN close[49];
int flag[7][7];

int closeSize; //close�Ĵ�С

//4�������ƶ�ʱ��ƫ����
const int xtran[4] = {-1, 0, 1, 0}; //�ϣ����£���
const int ytran[4] = {0, 1, 0, -1};


#endif //