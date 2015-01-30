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

int closeSize; //close的大小

//4个方向移动时的偏移量
const int xtran[4] = {-1, 0, 1, 0}; //上，左，下，右
const int ytran[4] = {0, 1, 0, -1};


#endif //