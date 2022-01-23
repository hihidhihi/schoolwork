#include<stdio.h>
#include <iostream>
#include <graphics.h>		
#include <conio.h>
#include <cstdio>
#include <cmath>
#include <time.h>
#include <list>
#include <algorithm>
#include <windows.h> 
#include <Mmsystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;
#if 1
#define  MAPH 7
#define  MAPW 10
#define  SLENGTH     10					//设置方格边长为10
#define  DLENGTH     14		//方格边长为10的情况下，对角线近似的取14
#define PGL 60
#define FLAMEFIXX 10
#define FLAMEFIXY 90
const TCHAR str[10][2] = { _T("0"), _T("1"), _T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8"), _T("9") };
typedef enum
{
	BLANK, WALL, ROAD, SIDE, MOB
}TYPE;
typedef struct GRID
{
	int x;
	int y;
	int G;
	int H;
	int F;
	int mobifright;
	int wallimg;
	int ifthorn = 0;
	int thornstate;
	int tthorn=0;
	struct GRID* prev;
	TYPE type;
}GRID, * PGRID;
typedef struct TORCH {
	int x, y, iflitten;
}TORCH;
typedef struct MYTIME
{
	int hour;
	int minute;
	int second;
}MYTIME;
typedef struct MYROAD
{
	int r[MAPH * MAPW];
	int length;//r数组的长度
	int count;//根据r计算出来的至少需要count步才能够完成游戏，由于计算出的路径有45°方向，但是游戏只允许上下左右四个方向，所以count>=length
}MYROAD;
typedef struct BONE {
	int vx, vy, w, image;
}BONE;
typedef struct BONELIST {
	BONE bone[15];
	int cbone, x, y;
	double f = 20.0;
}BONELIST;
typedef struct KICKFX {
	int x, y,k;
	double f = 4.0;
}KICKFX;
//以下为高海峰的全局变量
int W_MAX, M_MAX, T_MAX;
int x = 45;
int y = 0;
int timediff = 0, tstart, tend;
IMAGE a[403],b[50];
BONELIST bonelist[10];
KICKFX kickfx[4];
int fmove = 6, step, ifend = 0, skill = 3, ifright = 1, c, tmob = 0,tdust,tkick,tblood,sblood=0,t11,t12;
double f = 0, fkick = 7, fmob = 6, fskill = 9, fskillhero = 6, fdust,fthorn=4,fblood=6;
PGRID pHero, pEnd;
GRID pMap[MAPH * MAPW];
TORCH ptorch1[4];
//以下为王天驹的全局变量
IMAGE bkimg;
IMAGE maskimg;
IMAGE srcimg;
IMAGE bkimg1, bkimg2, bkimg3, bkimg4, bkimg5, bkimg6, bkimg7, bkimg8, bkimg9, bkimg10;
int j;
int k;
int second = 0,pk=0;
//以下为A*算法
void getGridF(PGRID pGrid)
{
	int t;
	if (pGrid->prev != NULL)
	{
		t = abs(pGrid->x - pGrid->prev->x) +
			abs(pGrid->y - pGrid->prev->y);
		if (t == 2)
		{
			pGrid->G = pGrid->prev->G + DLENGTH;
		}
		else
		{
			pGrid->G = pGrid->prev->G + SLENGTH;
		}
	}
	pGrid->F = pGrid->G + pGrid->H;
}
void getMinOpenlist(list<PGRID>* OpenList)
{
	PGRID p, s;
	p = OpenList->front();
	OpenList->pop_front();
	for (int i = 0; i < OpenList->size(); i++)
	{
		s = OpenList->front();
		OpenList->pop_front();
		if (s->F < p->F)
		{
			OpenList->push_back(p);
			p = s;
		}
		else
		{
			OpenList->push_back(s);
		}
	}
	OpenList->push_front(p);
}
void findNeighbor(GRID pNeighbor[], PGRID pCur)
{
	pNeighbor[0].x = pCur->x + 1;
	pNeighbor[0].y = pCur->y;
	pNeighbor[1].x = pCur->x + 1;
	pNeighbor[1].y = pCur->y + 1;
	pNeighbor[2].x = pCur->x;
	pNeighbor[2].y = pCur->y + 1;
	pNeighbor[3].x = pCur->x - 1;
	pNeighbor[3].y = pCur->y + 1;
	pNeighbor[4].x = pCur->x - 1;
	pNeighbor[4].y = pCur->y;
	pNeighbor[5].x = pCur->x - 1;
	pNeighbor[5].y = pCur->y - 1;
	pNeighbor[6].x = pCur->x;
	pNeighbor[6].y = pCur->y - 1;
	pNeighbor[7].x = pCur->x + 1;
	pNeighbor[7].y = pCur->y - 1;
}
void isAccess(GRID pNeighbor[], PGRID pMap)
{
	int i;

	for (i = 0; i < 8; i++)
	{
		//如果下标越界，就在列表中删除该格
		if (pNeighbor[i].x >= MAPW || pNeighbor[i].x < 0 || pNeighbor[i].y >= MAPH || pNeighbor[i].y < 0)
		{
			pNeighbor[i].x = -1;
			pNeighbor[i].y = -1;
		}
		//判断该格是否为障碍物
		else
		{
			if (pMap[pNeighbor[i].y * MAPW + pNeighbor[i].x].type != BLANK)
			{
				//如果障碍物位于当前格的1位置，那么2,8应该被忽略
				if (i == 0)
				{
					pNeighbor[1].x = -1;
					pNeighbor[1].y = -1;
					pNeighbor[7].x = -1;
					pNeighbor[7].y = -1;
				}
				//如果障碍物位于start的3位置，那么2,4应该被忽略
				else if (i == 2)
				{
					pNeighbor[1].x = -1;
					pNeighbor[1].y = -1;
					pNeighbor[3].x = -1;
					pNeighbor[3].y = -1;
				}
				//如果障碍物位于start点的5位置，那么4,6应该被忽略
				else if (i == 4)
				{
					pNeighbor[3].x = -1;
					pNeighbor[3].y = -1;
					pNeighbor[5].x = -1;
					pNeighbor[5].y = -1;
				}
				//如果障碍物位于start点的7位置，那么6,8应该被忽略
				else if (i == 6)
				{
					pNeighbor[5].x = -1;
					pNeighbor[5].y = -1;
					pNeighbor[7].x = -1;
					pNeighbor[7].y = -1;
				}
				pNeighbor[i].x = -1;
				pNeighbor[i].y = -1;
			}
		}
	}
}
int isNeiInList(PGRID pNeighbor, list<PGRID> List)
{
	list<PGRID>::iterator itr;
	PGRID p;
	int t = 0;
	for (itr = List.begin(); itr != List.end(); itr++)
	{
		p = *itr;
		if (p->x == pNeighbor->x && p->y == pNeighbor->y)
		{
			t = 1;
		}
	}
	return t;
}
int isInList(PGRID pGrid, list<PGRID> List)
{
	list<PGRID>::iterator itr;
	itr = find(List.begin(), List.end(), pGrid);
	if (itr == List.end())
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
void getGridGF(PGRID p)
{
	int t;

	if (p->prev != NULL)
	{
		t = abs(p->x - p->prev->x) + abs(p->y - p->prev->y);
		if (t == 2)
		{
			p->G = p->prev->G + DLENGTH;
		}
		else
		{
			p->G = p->prev->G + SLENGTH;
		}
	}
	else
	{
		p->G = 0;
	}
	p->F = p->G + p->H;
}
bool CompareF(const PGRID& p1, const PGRID& p2)
{
	return p1->F < p2->F;
}
bool getRoad(PGRID pMap, PGRID pBegin, PGRID pEnd)
{
	list<PGRID> OpenList;
	list<PGRID> CloseList;
	list<PGRID> RoadList;

	PGRID pCur;
	PGRID p;
	GRID pNeighbor[8];

	int i;
	int G;
	int t;

	//把起点加入openlist
	getGridF(pBegin);
	pBegin->prev = NULL;
	OpenList.push_back(pBegin);
	pCur = OpenList.front();

	while (!(isInList(pEnd, OpenList) || OpenList.empty()))
	{
		//取出openlist中F值最小的方格，为当前格。
		getMinOpenlist(&OpenList);
		pCur = OpenList.front();

		//把它切换到关闭列表。
		OpenList.pop_front();
		CloseList.push_back(pCur);

		//找到当前格相邻的8格
		findNeighbor(pNeighbor, pCur);
		isAccess(pNeighbor, pMap);

		for (i = 0; i < 8; i++)
		{
			//对于相邻的8格中的每一个格子，如果它不可通过或者已经在关闭列表中，略过它
			if ((-1 == pNeighbor[i].x && -1 == pNeighbor[i].y) || isNeiInList(&pNeighbor[i], CloseList))
			{
				;
			}
			else
			{
				p = &pMap[pNeighbor[i].y * MAPW + pNeighbor[i].x];
				//如果它不在开启列表中，把它添加进去。把当前格作为这一格的父节点。记录这一格的F,G,和H值。
				if (!isNeiInList(&pNeighbor[i], OpenList))
				{
					p->prev = pCur;
					getGridGF(p);
					OpenList.push_back(p);
				}
				//如果它已经在开启列表中，用G值为参考检查新的路径是否更好。
				else
				{
					t = abs(p->x - pCur->x) + abs(p->y - pCur->y);
					if (t == 2)
					{
						G = pCur->G + DLENGTH;
					}
					else
					{
						G = pCur->G + SLENGTH;
					}
					if (G < p->G)
					{
						p->prev = pCur;
						getGridGF(p);
					}
				}
			}
		}
	}
	if (isInList(pEnd, OpenList))
	{
		RoadList.push_front(pEnd);
		pCur = RoadList.front();
		while (pCur->prev != pBegin)
		{
			pCur = pCur->prev;
			pCur->type = ROAD;
			RoadList.push_front(pCur);
		}
		RoadList.push_front(pBegin);
	}
	else
	{
		printf("迷宫无解\n");
		return false;
	}
	//		PRINTMAP(pMap);
	return true;
}
//以上为A*算法
//以下为高海峰的代码（金澄城的夹杂其中）
void InitMap()
{
	int i, j;
	for (i = 0; i < MAPH; i++)
	{
		for (j = 0; j < MAPW; j++)
		{
			pMap[i * MAPW + j].x = j;
			pMap[i * MAPW + j].y = i;
			pMap[i * MAPW + j].G = 0;
			pMap[i * MAPW + j].H = 0;
			pMap[i * MAPW + j].F = 0;
			pMap[i * MAPW + j].prev = NULL;
			pMap[i * MAPW + j].type = BLANK;
		}
	}
}
void creatSide()
{
	for (int i = 0; i < MAPW; i++)
	{
		pMap[i].type = SIDE;
		pMap[i + MAPW * (MAPH - 1)].type = SIDE;
	}
	for (int i = MAPW; i < MAPH * MAPW; i += MAPW) {
		pMap[i].type = SIDE;
		pMap[i + MAPW - 1].type = SIDE;
	}
	pMap[12].type = SIDE;
	pMap[16].type = SIDE;
	pMap[17].type = SIDE;
	pMap[18].type = SIDE;
	pMap[28].type = SIDE;
	pMap[51].type = SIDE;
	pMap[57].type = SIDE;
}
void creatWall()
{
	int r, i;
	for (i = 0; i < W_MAX; i++)
	{
		r = rand() % (MAPH * MAPW);
		while (pMap[r].type != BLANK)
		{
			r = (r + rand() % (MAPH * MAPW)) % (MAPH * MAPW);
		}
		pMap[r].type = WALL;
		pMap[r].wallimg = rand() % 10;
	}
}
void creatThorn()
{
	int r, i;
	for (i = 0; i < T_MAX; i++)
	{
		r = rand() % (MAPH * MAPW);
		while (pMap[r].type != ROAD&& pMap[r].type != MOB)
		{
			r = (r + rand() % (MAPH * MAPW)) % (MAPH * MAPW);
		}
		pMap[r].ifthorn=1;
		pMap[r].thornstate = rand() % 4 + 2;
	}
}
void creatRoad() {
	for (int i = 0; i < MAPH * MAPW; i++)
		if (pMap[i].type == BLANK)
			pMap[i].type = ROAD;
}
void creatTorch1() {
	ptorch1->x = 268;
	ptorch1->y = 154;
	ptorch1->iflitten = rand() % 2;
	(ptorch1 + 1)->x = 668;
	(ptorch1 + 1)->y = 107;
	(ptorch1 + 1)->iflitten = rand() % 2;
	(ptorch1 + 2)->x = 868;
	(ptorch1 + 2)->y = 113;
	(ptorch1 + 2)->iflitten = rand() % 2;
	(ptorch1 + 3)->x = 968;
	(ptorch1 + 3)->y = 307;
	(ptorch1 + 3)->iflitten = rand() % 2;
}
PGRID chooseStart()
{
	int flag = 1;
	int r = 0;
	while (flag && r < MAPH * MAPW)
	{
		if (pMap[r].type == BLANK)flag = 0;
		else r++;
	}
	return &pMap[r];
}
PGRID chooseEnd()
{
	int flag = 1;
	int r = MAPH * MAPW - 1;
	while (flag && r >= 0)
	{
		if (BLANK == pMap[r].type)flag = 0;
		else r--;
	}
	//计算每一个点的H
	int k;
	int l;
	for (k = 0; k < MAPH; k++)
	{
		for (l = 0; l < MAPW; l++)
		{
			pMap[k * MAPW + l].H = (abs(pMap[k * MAPW + l].x - pMap[r].x) +
				abs(pMap[k * MAPW + l].y - pMap[r].y)) * SLENGTH;
		}
	}
	return &pMap[r];
}
void creat() {
	creatSide();
	creatWall();
	creatTorch1();
	pHero = chooseStart();
	pEnd = chooseEnd();
}
void creatMob() {
	int r;
	for (int mob = 0; mob < M_MAX; mob++)
	{
		r = rand() % (MAPH * MAPW);
		while (pMap[r].type != ROAD || pMap + r == pHero || pMap + r == pEnd)
		{
			r = (r + rand() % (MAPH * MAPW)) % (MAPH * MAPW);
		}
		pMap[r].type = MOB;
		pMap[r].mobifright = rand() % 2;
	}
}
void DrawThorn() {
	int y5 = -20;
	for (int i = 0; i < MAPH * MAPW;i++) {
		if (pMap[i].ifthorn == 1) {
			if (pMap[i].tthorn == 0&&fthorn < 4) {
				putimage(x + pMap[i].x * 100, y + pMap[i].y * 100+ y5, a + 399 + (int)fthorn, SRCAND);
				putimage(x + pMap[i].x * 100, y + pMap[i].y * 100 + y5, a + 391 + (int)fthorn, SRCPAINT);
			}		
			else if (pMap[i].tthorn == 0 && fthorn >=4) {
				putimage(x + pMap[i].x * 100, y + pMap[i].y * 100 + y5, a + 403 , SRCAND);
				putimage(x + pMap[i].x * 100, y + pMap[i].y * 100 + y5, a + 394 , SRCPAINT);
			}
			else if (pMap[i].tthorn == 1 && fthorn < 4) {
				putimage(x + pMap[i].x * 100, y + pMap[i].y * 100 + y5, a + 395 + (int)fthorn, SRCAND);
				putimage(x + pMap[i].x * 100, y + pMap[i].y * 100 + y5, a + 387 + (int)fthorn, SRCPAINT);
			}
			else {
				putimage(x + pMap[i].x * 100, y + pMap[i].y * 100 + y5, a + 399 , SRCAND);
				putimage(x + pMap[i].x * 100, y + pMap[i].y * 100 + y5, a + 391 , SRCPAINT);
			}
			if (fthorn < 4)fthorn += 0.083;
		}
	}
}
void DrawDust(int x1,int y1) {
	if (fdust < 3) {
		int y2 = y1 + 25;
		switch (tdust)
		{
		case 0:
			putimage(x1, y2, a + 370 + (int)fdust, SRCAND);
			putimage(x1, y2, a + 353 + (int)fdust, SRCPAINT);
			break;
		case 1:
			putimage(x1, y2, a + 373 + (int)fdust, SRCAND);
			putimage(x1, y2, a + 356 + (int)fdust, SRCPAINT);
			break;
		case 2:
			putimage(x1, y2, a + 376 + (int)fdust, SRCAND);
			putimage(x1, y2, a + 359 + (int)fdust, SRCPAINT);
			break;
		}
	}
	fdust+=0.5;
}
void DrawKickFx() {
	for (int i = 0; i < 4; i++) {
		if (kickfx[i].f < 4)
		{
			putimage(x+kickfx[i].x*100, y+kickfx[i].y*100+35, a + 379 + kickfx[i] .k*4+ (int)kickfx[i].f, SRCAND);
			putimage(x + kickfx[i].x * 100, y + kickfx[i].y * 100+35, a + 362 + kickfx[i].k * 4+ (int)kickfx[i].f, SRCPAINT);
		}
		kickfx[i].f+=0.5;
	}
}
void DrawWall(int x, int y,int img) {
	putimage(x, y, a + 343 + img, SRCAND);
	putimage(x, y, a + 333 + img, SRCPAINT);
}
void DrawHeroMove() {
	if (c == 1) {
		putimage(x + pHero->x * 100 + (fmove + 1) * 14 - 100, y + pHero->y * 100, a + 131 + fmove, SRCAND);
		putimage(x + pHero->x * 100 + (fmove + 1) * 14 - 100, y + pHero->y * 100, a + 125 + fmove, SRCPAINT);
		DrawDust(x + pHero->x * 100 - 100, y + pHero->y * 100);
	}
	if (c == MAPW && ifright == 1) {
		putimage(x + pHero->x * 100, y + pHero->y * 100 + (fmove + 1) * 14 - 100, a + 131 + fmove, SRCAND);
		putimage(x + pHero->x * 100, y + pHero->y * 100 + (fmove + 1) * 14 - 100, a + 125 + fmove, SRCPAINT);
		DrawDust(x + pHero->x * 100, y + pHero->y * 100-100);
	}
	if (c == MAPW && ifright == 0) {
		putimage(x + pHero->x * 100, y + pHero->y * 100 + (fmove + 1) * 14 - 100, a + 143 + fmove, SRCAND);
		putimage(x + pHero->x * 100, y + pHero->y * 100 + (fmove + 1) * 14 - 100, a + 137 + fmove, SRCPAINT);
		DrawDust(x + pHero->x * 100, y + pHero->y * 100 - 100);
	}
	if (c == -1) {
		putimage(x + pHero->x * 100 - (fmove + 1) * 14 + 100, y + pHero->y * 100, a + 143 + fmove, SRCAND);
		putimage(x + pHero->x * 100 - (fmove + 1) * 14 + 100, y + pHero->y * 100, a + 137 + fmove, SRCPAINT);
		DrawDust(x + pHero->x * 100 + 100, y + pHero->y * 100);
	}
	if (c == -MAPW && ifright == 1) {
		putimage(x + pHero->x * 100, y + pHero->y * 100 - (fmove + 1) * 14 + 100, a + 131 + fmove, SRCAND);
		putimage(x + pHero->x * 100, y + pHero->y * 100 - (fmove + 1) * 14 + 100, a + 125 + fmove, SRCPAINT);
		DrawDust(x + pHero->x * 100, y + pHero->y * 100 + 100);
	}
	if (c == -MAPW && ifright == 0) {
		putimage(x + pHero->x * 100, y + pHero->y * 100 - (fmove + 1) * 14 + 100, a + 143 + fmove, SRCAND);
		putimage(x + pHero->x * 100, y + pHero->y * 100 - (fmove + 1) * 14 + 100, a + 137 + fmove, SRCPAINT);
		DrawDust(x + pHero->x * 100, y + pHero->y * 100 + 100);
	}
}
void DrawHeroStand() {
	if (ifright == 1) {
		putimage(x + pHero->x * 100, y + pHero->y * 100, a + 12 + (int)f, SRCAND);
		putimage(x + pHero->x * 100, y + pHero->y * 100, a + (int)f, SRCPAINT);
	}
	if (ifright == 0) {
		putimage(x + pHero->x * 100, y + pHero->y * 100, a + 161 + (int)f, SRCAND);
		putimage(x + pHero->x * 100, y + pHero->y * 100, a + 149 + (int)f, SRCPAINT);
	}

}
void DrawHeroSkill() {
	if (ifright == 1)
	{
		putimage(x + pHero->x * 100, y + pHero->y * 100, a + 302 + (int)fskillhero, SRCAND);
		putimage(x + pHero->x * 100, y + pHero->y * 100, a + 283 + (int)fskillhero, SRCPAINT);
	}
	if (ifright == 0) {
		putimage(x + pHero->x * 100, y + pHero->y * 100, a + 327 + (int)fskillhero, SRCAND);
		putimage(x + pHero->x * 100, y + pHero->y * 100, a + 321 + (int)fskillhero, SRCPAINT);
	}
}
void DrawHeroKick() {
	if (ifright == 1)
	{
		putimage(x + pHero->x * 100, y + pHero->y * 100, a + 180 + (int)fkick, SRCAND);
		putimage(x + pHero->x * 100, y + pHero->y * 100, a + 173 + (int)fkick, SRCPAINT);
	}
	if (ifright == 0) {
		putimage(x + pHero->x * 100, y + pHero->y * 100, a + 194 + (int)fkick, SRCAND);
		putimage(x + pHero->x * 100, y + pHero->y * 100, a + 187 + (int)fkick, SRCPAINT);
	}
}
void DrawHero() {
	if (fmove < 6)
	{
		fkick = 7;
		fskillhero = 6;
		DrawHeroMove();
		fmove++;
	}
	else if (fskillhero < 6 && fkick != 0)
	{
		fkick = 7;
		DrawHeroSkill();
		fskillhero += 0.5;
	}
	else if (fkick < 7)
	{
		DrawHeroKick();
		fkick += 0.5;
	}
	else DrawHeroStand();
}
void Transition1(void (*pfunc)()) {
	mciSendString(_T("open 开门.mp3 alias opendoor"), NULL, 0, NULL);
	mciSendString(_T("play opendoor"), NULL, 0, NULL);
	IMAGE g[22], h[22];
	TCHAR str[22][100];
	for (int i = 0; i < 22; i++)
	{
		_sntprintf_s(str[i], sizeof(str[i]), _T("1\\transition00%02d 拷贝.png"), i + 1);
		loadimage(g + i, str[i]);
	}
	for (int i = 0; i < 22; i++)
	{
		_sntprintf_s(str[i], sizeof(str[i]), _T("2\\transition00%02d 拷贝.png"), i + 1);
		loadimage(h + i, str[i]);
	}
	for (int i = 0; i < 22; i++)
	{
		pfunc();
		putimage(0, 0, h + i, SRCAND);
		putimage(0, 0, g + i, SRCPAINT);
		FlushBatchDraw();
		Sleep(50);
		cleardevice();
		if (f >= 11)f = 0;
		else f++;
	}
}
void Transition2(void (*pfunc)()) {
	IMAGE g[9], h[9];
	TCHAR str[9][100];
	for (int i = 0; i < 9; i++)
	{
		_sntprintf_s(str[i], sizeof(str[i]), _T("1\\transition00%02d 拷贝.png"), i + 23);
		loadimage(g + i, str[i]);
	}
	for (int i = 0; i < 9; i++)
	{
		_sntprintf_s(str[i], sizeof(str[i]), _T("2\\transition00%02d 拷贝.png"), i + 23);
		loadimage(h + i, str[i]);
	}
	for (int i = 0; i < 9; i++)
	{
		pfunc();
		putimage(0, 0, h + i, SRCAND);
		putimage(0, 0, g + i, SRCPAINT);
		FlushBatchDraw();
		Sleep(50);
		cleardevice();
		if (f >= 11)f = 0;
		else f++;
	}
	mciSendString(_T("close opendoor"), NULL, 0, NULL);
}
void InitResource()
{
	TCHAR str[20][100];
	int i;
	for (i = 0; i < 12; i++)
	{
		_sntprintf_s(str[i], sizeof(str[i]), _T("hero00%02d.png"), i + 22);
		loadimage(a + i, str[i]);
	}
	for (; i < 24; i++)
	{
		_sntprintf_s(str[i - 12], sizeof(str[i - 12]), _T("hero00%02d1.png"), i + 22 - 12);
		loadimage(a + i, str[i - 12]);
	}
	loadimage(a + (i++), _T("chapterBG0004.png"));
	loadimage(a + (i++), _T("boxExport0001.png"));
	loadimage(a + (i++), _T("boxExport00011.png"));
	loadimage(a + (i++), _T("1\\step.png"));
	loadimage(a + (i++), _T("2\\stepy.png"));
	loadimage(a + (i++), _T("1\\skill 拷贝.png"));
	loadimage(a + (i++), _T("2\\skill 拷贝.png"));
	for (; i < 49; i++) {
		_sntprintf_s(str[i - 31], sizeof(str[i - 31]), _T("death_P200%02d.png"), i - 31 + 1);
		loadimage(a + i, str[i - 31]);
	}
	for (; i < 61; i++) {
		_sntprintf_s(str[i - 49], sizeof(str[i - 49]), _T("1\\assets100V202%d 拷贝.png"), i - 49 + 25);
		loadimage(a + i, str[i - 49]);
	}
	for (; i < 73; i++) {
		_sntprintf_s(str[i - 61], sizeof(str[i - 61]), _T("2\\assets100V202%d 拷贝.png"), i - 61 + 25);
		loadimage(a + i, str[i - 61]);
	}
	for (; i < 85; i++) {
		_sntprintf_s(str[i - 73], sizeof(str[i - 73]), _T("1\\lucy_finalModel00%02d 拷贝.png"), i - 73 + 1);
		loadimage(a + i, str[i - 73]);
	}
	for (; i < 97; i++) {
		_sntprintf_s(str[i - 85], sizeof(str[i - 85]), _T("2\\lucy_finalModel00%02d 拷贝.png"), i - 85 + 1);
		loadimage(a + i, str[i - 85]);
	}
	for (; i < 109; i++) {
		_sntprintf_s(str[i - 97], sizeof(str[i - 97]), _T("1\\fire00%02d 拷贝.png"), i - 97 + 1);
		loadimage(a + i, str[i - 97]);
	}
	for (; i < 121; i++) {
		_sntprintf_s(str[i - 109], sizeof(str[i - 109]), _T("2\\fire00%02d 拷贝.png"), i - 109 + 1);
		loadimage(a + i, str[i - 109]);
	}
	loadimage(a + (i++), _T("1\\FLAMEbase0001 拷贝.png"));
	loadimage(a + (i++), _T("2\\FLAMEbase0001 拷贝.png"));
	loadimage(a + (i++), _T("1\\FLAMEbase0002 拷贝.png"));
	loadimage(a + (i++), _T("2\\FLAMEbase0002 拷贝.png"));
	for (; i < 131; i++)
	{
		_sntprintf_s(str[i - 125], sizeof(str[i - 125]), _T("1\\assets100V200%d 拷贝.png"), i - 125 + 53);
		loadimage(a + i, str[i - 125]);
	}
	for (; i < 137; i++)
	{
		_sntprintf_s(str[i - 131], sizeof(str[i - 131]), _T("2\\assets100V200%d 拷贝.png"), i - 131 + 53);
		loadimage(a + i, str[i - 131]);
	}
	for (; i < 143; i++)
	{
		_sntprintf_s(str[i - 137], sizeof(str[i - 137]), _T("1\\assets100V200%d.png"), i - 137 + 53);
		loadimage(a + i, str[i - 137]);
	}
	for (; i < 149; i++)
	{
		_sntprintf_s(str[i - 143], sizeof(str[i - 143]), _T("2\\assets100V200%d.png"), i - 143 + 53);
		loadimage(a + i, str[i - 143]);
	}
	for (; i < 161; i++)
	{
		_sntprintf_s(str[i - 149], sizeof(str[i - 149]), _T("1\\hero00%d 拷贝.png"), i - 149 + 22);
		loadimage(a + i, str[i - 149]);
	}
	for (; i < 173; i++)
	{
		_sntprintf_s(str[i - 161], sizeof(str[i - 161]), _T("2\\hero00%d 拷贝.png"), i - 161 + 22);
		loadimage(a + i, str[i - 161]);
	}
	for (; i < 180; i++) {
		_sntprintf_s(str[i - 173], sizeof(str[i - 173]), _T("1\\hero00%d.png"), i - 173 + 40);
		loadimage(a + i, str[i - 173]);
	}
	for (; i < 187; i++) {
		_sntprintf_s(str[i - 180], sizeof(str[i - 180]), _T("2\\hero00%d.png"), i - 180 + 40);
		loadimage(a + i, str[i - 180]);
	}
	for (; i < 194; i++) {
		_sntprintf_s(str[i - 187], sizeof(str[i - 187]), _T("1\\hero00%d 拷贝.png"), i - 187 + 40);
		loadimage(a + i, str[i - 187]);
	}
	for (; i < 201; i++) {
		_sntprintf_s(str[i - 194], sizeof(str[i - 194]), _T("2\\hero00%d 拷贝.png"), i - 194 + 40);
		loadimage(a + i, str[i - 194]);
	}
	for (; i < 207; i++) {
		_sntprintf_s(str[i - 201], sizeof(str[i - 201]), _T("1\\assets100V20%d 拷贝.png"), i - 201 + 198);
		loadimage(a + i, str[i - 201]);
	}//右向怪受伤
	for (; i < 213; i++) {
		_sntprintf_s(str[i - 207], sizeof(str[i - 207]), _T("2\\assets100V20%d 拷贝.png"), i - 207 + 198);
		loadimage(a + i, str[i - 207]);
	}//右向怪受伤掩码
	for (; i < 219; i++) {
		_sntprintf_s(str[i - 213], sizeof(str[i - 213]), _T("1\\assets100V20%d.png"), i - 213 + 198);
		loadimage(a + i, str[i - 213]);
	}//左向怪受伤
	for (; i < 225; i++) {
		_sntprintf_s(str[i - 219], sizeof(str[i - 219]), _T("2\\assets100V20%d.png"), i - 219 + 198);
		loadimage(a + i, str[i - 219]);
	}//左向怪受伤掩码
	for (; i < 237; i++) {
		_sntprintf_s(str[i - 225], sizeof(str[i - 225]), _T("1\\assets100V202%d.png"), i - 225 + 25);
		loadimage(a + i, str[i - 225]);
	}//左向怪
	for (; i < 249; i++) {
		_sntprintf_s(str[i - 237], sizeof(str[i - 237]), _T("2\\assets100V202%d.png"), i - 237 + 25);
		loadimage(a + i, str[i - 237]);
	}//左向怪掩码
	loadimage(a + (i++), _T("1\\珍珠1.2.png"));
	loadimage(a + (i++), _T("2\\珍珠1.2.png"));
	for (; i < 260; i++) {
		_sntprintf_s(str[i - 251], sizeof(str[i - 251]), _T("1\\huge_vfx000%d 拷贝.png"), i - 251 + 1);
		loadimage(a + i, str[i - 251]);
	}
	for (; i < 269; i++) {
		_sntprintf_s(str[i - 260], sizeof(str[i - 260]), _T("2\\huge_vfx000%d 拷贝.png"), i - 260 + 1);
		loadimage(a + i, str[i - 260]);
	}
	for (; i < 276; i++) {
		_sntprintf_s(str[i - 269], sizeof(str[i - 269]), _T("1\\particle000%d 拷贝.png"), i - 269 + 1);
		loadimage(a + i, str[i - 269]);
	}
	for (; i < 283; i++) {
		_sntprintf_s(str[i - 276], sizeof(str[i - 276]), _T("2\\particle000%d 拷贝.png"), i - 276 + 1);
		loadimage(a + i, str[i - 276]);
	}
	for (; i < 302; i++) {
		_sntprintf_s(str[i - 283], sizeof(str[i - 283]), _T("1\\hero00%d 拷贝.png"), i - 283 + 59);
		loadimage(a + i, str[i - 283]);
	}
	for (; i < 321; i++) {
		_sntprintf_s(str[i - 302], sizeof(str[i - 302]), _T("2\\hero00%d 拷贝.png"), i - 302 + 59);
		loadimage(a + i, str[i - 302]);
	}
	for (; i < 327; i++) {
		_sntprintf_s(str[i - 321], sizeof(str[i - 321]), _T("1\\hero00%d.png"), i - 321 + 59);
		loadimage(a + i, str[i - 321]);
	}
	for (; i < 333; i++) {
		_sntprintf_s(str[i - 327], sizeof(str[i - 327]), _T("2\\hero00%d.png"), i - 327 + 59);
		loadimage(a + i, str[i - 327]);
	}
	for (; i < 343; i++) {
		_sntprintf_s(str[i - 333], sizeof(str[i - 333]), _T("1\\boxExport00%02d 拷贝.png"), i - 333 + 1);
		loadimage(a + i, str[i - 333]);
	}
	for (; i < 353; i++) {
		_sntprintf_s(str[i - 343], sizeof(str[i - 343]), _T("2\\boxExport00%02d 拷贝.png"), i - 343 + 1);
		loadimage(a + i, str[i - 343]);
	}
	for (; i < 370; i++) {
		_sntprintf_s(str[i - 353], sizeof(str[i - 353]), _T("1\\small_vfx00%02d 拷贝.png"), i - 353 + 1);
		loadimage(a + i, str[i - 353]);
	}
	for (; i < 387; i++) {
		_sntprintf_s(str[i - 370], sizeof(str[i - 370]), _T("2\\small_vfx00%02d 拷贝.png"), i - 370 + 1);
		loadimage(a + i, str[i - 370]);
	}
	for (; i < 395; i++) {
		_sntprintf_s(str[i - 387], sizeof(str[i - 387]), _T("1\\assets100V201%d 拷贝.png"), i - 387 + 16);
		loadimage(a + i, str[i - 387]);
	}
	for (; i < 403; i++) {
		_sntprintf_s(str[i - 395], sizeof(str[i - 395]), _T("2\\assets100V201%d 拷贝.png"), i - 395 + 16);
		loadimage(a + i, str[i - 395]);
	}
	i = 0;
	for (; i < 18; i++) {
		_sntprintf_s(str[i], sizeof(str[i]), _T("1\\huge_vfx00%d 拷贝.png"), i + 10);
		loadimage(b + i, str[i]);
	}
	for (; i < 36; i++) {
		_sntprintf_s(str[i - 18], sizeof(str[i - 18]), _T("2\\huge_vfx00%d 拷贝.png"), i - 18 + 10);
		loadimage(b + i, str[i -18]);
	}
}
void InitKickFx(int mstep) {
	kickfx[tkick].k = rand() % 2;
	kickfx[tkick].f = 0;
	kickfx[tkick].x = (pHero + mstep)->x;
	kickfx[tkick].y = (pHero + mstep)->y;
	tkick++;
	if (tkick == 4)tkick = 0;
}
void InitKillMob(int mstep) {
	(pHero + mstep)->type = ROAD;
	InitKickFx(mstep);
	bonelist[tmob].f = 0;
	bonelist[tmob].x = (pHero + mstep)->x;
	bonelist[tmob].y = (pHero + mstep)->y;
	tmob++;
	if (tmob == 10)tmob = 0;
}
void DrawFire1() {
	for (int i = 0; i < 4; i++)if ((ptorch1 + i)->iflitten) {
		putimage((ptorch1 + i)->x - FLAMEFIXX, (ptorch1 + i)->y - FLAMEFIXY, a + 122, SRCAND);
		putimage((ptorch1 + i)->x - FLAMEFIXX, (ptorch1 + i)->y - FLAMEFIXY, a + 121, SRCPAINT);
		putimage((ptorch1 + i)->x - 4, (ptorch1 + i)->y - FLAMEFIXY - 37, a + 109 + (int)f, SRCAND);
		putimage((ptorch1 + i)->x - 4, (ptorch1 + i)->y - FLAMEFIXY - 37, a + 97 + (int)f, SRCPAINT);
	}
	else {
		putimage((ptorch1 + i)->x - FLAMEFIXX, (ptorch1 + i)->y - FLAMEFIXY, a + 124, SRCAND);
		putimage((ptorch1 + i)->x - FLAMEFIXX, (ptorch1 + i)->y - FLAMEFIXY, a + 123, SRCPAINT);
	}
}
void DrawBone(int t) {
	if (bonelist[t].f == 0) {
		bonelist[t].cbone = rand() % 3 + 5;
		for (int i = 0; i < bonelist[t].cbone; i++)
		{
			bonelist[t].bone[i].vx = rand() % 81 - 40;
			bonelist[t].bone[i].vy = rand() % 100 - 90;
			bonelist[t].bone[i].image = rand() % 7;
			bonelist[t].bone[i].w = rand() % 11 - 5;
		}
	}
	for (int i = 0; i < bonelist[t].cbone; i++) {
		IMAGE rotate1, rotate2;
		rotateimage(&rotate1, a + 269 + bonelist[t].bone[i].image, bonelist[t].bone[i].w * bonelist[t].f, BLACK, false, false);
		rotateimage(&rotate2, a + 276 + bonelist[t].bone[i].image, bonelist[t].bone[i].w * bonelist[t].f, WHITE, false, false);
		putimage(x+20 + bonelist[t].x * 100 + bonelist[t].bone[i].vx * bonelist[t].f, y+60 + bonelist[t].y * 100 + bonelist[t].bone[i].vy * bonelist[t].f + 5 * bonelist[t].f * bonelist[t].f, &rotate2, SRCAND);
		putimage(x+20 + bonelist[t].x * 100 + bonelist[t].bone[i].vx * bonelist[t].f, y +60+ bonelist[t].y * 100 + bonelist[t].bone[i].vy * bonelist[t].f + 5 * bonelist[t].f * bonelist[t].f, &rotate1, SRCPAINT);
	}
	bonelist[t].f += 0.5;
}
void DrawItem() {
	for (int i = 0; i < MAPH * MAPW; i++)
	{
		if (pMap[i].type == WALL)DrawWall(45 + pMap[i].x * 100, 30 + pMap[i].y * 100, pMap[i].wallimg);
		if ((pMap + i)->type == MOB && ((pMap + i) == pHero + c * 2) && fmob < 6)
		{
			switch (c)
			{
			case -1:
				putimage(x + (pMap + i + 1)->x * 100 - (fmob + 1) * 14, y + (pMap + i)->y * 100, a + 207 + (int)fmob, SRCAND);
				putimage(x + (pMap + i + 1)->x * 100 - (fmob + 1) * 14, y + (pMap + i)->y * 100, a + 201 + (int)fmob, SRCPAINT);
				break;
			case 1:
				putimage(x + (pMap + i - 1)->x * 100 + (fmob + 1) * 14, y + (pMap + i)->y * 100, a + 219 + (int)fmob, SRCAND);
				putimage(x + (pMap + i - 1)->x * 100 + (fmob + 1) * 14, y + (pMap + i)->y * 100, a + 213 + (int)fmob, SRCPAINT);
				break;
			case -MAPW:
				if ((pMap + i)->mobifright == 1)
				{
					putimage(x + (pMap + i)->x * 100, y + (pMap + i + MAPW)->y * 100 - (fmob + 1) * 14, a + 207 + (int)fmob, SRCAND);
					putimage(x + (pMap + i)->x * 100, y + (pMap + i + MAPW)->y * 100 - (fmob + 1) * 14, a + 201 + (int)fmob, SRCPAINT);
				}
				else {
					putimage(x + (pMap + i)->x * 100, y + (pMap + i + MAPW)->y * 100 - (fmob + 1) * 14, a + 219 + (int)fmob, SRCAND);
					putimage(x + (pMap + i)->x * 100, y + (pMap + i + MAPW)->y * 100 - (fmob + 1) * 14, a + 213 + (int)fmob, SRCPAINT);
				}
				break;
			case MAPW:
				if ((pMap + i)->mobifright == 1)
				{
					putimage(x + (pMap + i)->x * 100, y + (pMap + i - MAPW)->y * 100 + (fmob + 1) * 14, a + 207 + (int)fmob, SRCAND);
					putimage(x + (pMap + i)->x * 100, y + (pMap + i - MAPW)->y * 100 + (fmob + 1) * 14, a + 201 + (int)fmob, SRCPAINT);
				}
				else {
					putimage(x + (pMap + i)->x * 100, y + (pMap + i - MAPW)->y * 100 + (fmob + 1) * 14, a + 219 + (int)fmob, SRCAND);
					putimage(x + (pMap + i)->x * 100, y + (pMap + i - MAPW)->y * 100 + (fmob + 1) * 14, a + 213 + (int)fmob, SRCPAINT);
				}
				break;
			}
			fmob += 0.6;
		}
		else if ((pMap + i)->type == MOB)
		{
			if ((pMap + i)->mobifright == 1)
			{
				putimage(x + (pMap + i)->x * 100, y + (pMap + i)->y * 100, a + 61 + (int)f, SRCAND);
				putimage(x + (pMap + i)->x * 100, y + (pMap + i)->y * 100, a + 49 + (int)f, SRCPAINT);
			}
			else {
				putimage(x + (pMap + i)->x * 100, y + (pMap + i)->y * 100, a + 237 + (int)f, SRCAND);
				putimage(x + (pMap + i)->x * 100, y + (pMap + i)->y * 100, a + 225 + (int)f, SRCPAINT);
			}
		}
	}
	for (int i = 0; i < 10; i++)
	{
		if (bonelist[i].f < 20)DrawBone(i);
	}

	DrawFire1();
}
void DrawStep() {
	putimage(1022, 351, a + 28, SRCAND);
	putimage(1022, 351, a + 27, SRCPAINT);
	LOGFONT stepfont;
	gettextstyle(&stepfont);
	stepfont.lfHeight = 70;
	_tcscpy_s(stepfont.lfFaceName, _T("Ravie"));
	stepfont.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&stepfont);
	settextcolor(WHITE);
	TCHAR str[10];
	_sntprintf_s(str, sizeof(str), _T("%d"), step);
	outtextxy(1135, 550, str);
}
void DrawSkill() {
	putimage(1016, 0, a + 30, SRCAND);
	putimage(1016, 0, a + 29, SRCPAINT);
	int skillfixx = -15, skillfixy = -13, skillfixx1 = -38, skillfixy1 = -31;
	switch (skill)
	{
	case 3:
		putimage(1145 + skillfixx, 24 + skillfixy, a + 250, SRCAND);
		putimage(1145 + skillfixx, 24 + skillfixy, a + 249, SRCPAINT);
	case 2:
		putimage(1200 + skillfixx, 64 + skillfixy, a + 250, SRCAND);
		putimage(1200 + skillfixx, 64 + skillfixy, a + 249, SRCPAINT);
	case 1:
		putimage(1250 + skillfixx, 150 + skillfixy, a + 250, SRCAND);
		putimage(1250 + skillfixx, 150 + skillfixy, a + 249, SRCPAINT);
	}
	if (fskill < 9) {
		if (skill == 2) {
			putimage(1145 + skillfixx1, 24 + skillfixy1, a + 260 + (int)fskill, SRCAND);
			putimage(1145 + skillfixx1, 24 + skillfixy1, a + 251 + (int)fskill, SRCPAINT);
		}
		if (skill == 1) {
			putimage(1200 + skillfixx1, 64 + skillfixy1, a + 260 + (int)fskill, SRCAND);
			putimage(1200 + skillfixx1, 64 + skillfixy1, a + 251 + (int)fskill, SRCPAINT);
		}
		if (skill == 0) {
			putimage(1250 + skillfixx1, 150 + skillfixy1, a + 260 + (int)fskill, SRCAND);
			putimage(1250 + skillfixx1, 150 + skillfixy1, a + 251 + (int)fskill, SRCPAINT);
		}
		fskill += 0.33;
	}
}
void Skill() {
	if ((pHero - 1)->type == MOB)InitKillMob(-1);
	if ((pHero + 1)->type == MOB)InitKillMob(1);
	if ((pHero - MAPW)->type == MOB)InitKillMob(-MAPW);
	if ((pHero + MAPW)->type == MOB)InitKillMob(MAPW);
}
void DrawDemon() {
	putimage(x + pEnd->x * 100, y + pEnd->y * 100 + 35, a + 85 + (int)f, SRCAND);
	putimage(x + pEnd->x * 100, y + pEnd->y * 100 + 35, a + 73 + (int)f, SRCPAINT);
}
void DrawTime()
{
	MYTIME mytime;
	tend = clock();
	timediff = (tend - tstart) / 1000;
	mytime.hour = timediff / 3600;
	timediff = timediff % 3600;
	mytime.minute = timediff / 60;
	mytime.second = timediff % 60;
	settextcolor(WHITE);
	settextstyle(40, 0, _T("Ravie"));
	int x2 = 870, y2 = 650;
	outtextxy(x2, y2, str[mytime.hour / 10]);
	outtextxy(x2 + 25, y2, str[mytime.hour % 10]);
	outtextxy(x2 + 45, y2 - 2, _T(":"));
	outtextxy(x2 + PGL, y2, str[mytime.minute / 10]);
	outtextxy(x2 + PGL + 25, y2, str[mytime.minute % 10]);
	outtextxy(x2 + 45 + PGL, y2 - 2, _T(":"));
	outtextxy(x2 + 2 * PGL, y2, str[mytime.second / 10]);
	outtextxy(x2 + 2 * PGL + 25, y2, str[mytime.second % 10]);
}
void DrawBlood() {
	if (fmove == 6&&sblood==1)
	{
		PlaySound(_T("受伤.wav"), NULL, SND_ASYNC);
		tblood = rand() % 3;
		fblood = 0;
		sblood = 0;
	}
	if (fblood < 6)
	{
		putimage(x + pHero->x * 100-110, y + pHero->y * 100, b + 18 + tblood * 6 + (int)fblood, SRCAND);
		putimage(x + pHero->x * 100-110, y + pHero->y * 100, b + tblood * 6 + (int)fblood, SRCPAINT);
	}
	fblood += 0.33;
}
void TThorn() {
	for (int i = 1; i < MAPH * MAPW; i++)
		if (pMap[i].ifthorn == 1) {
			pMap[i].tthorn++;
			if (pMap[i].tthorn == pMap[i].thornstate)pMap[i].tthorn = 0;
			if (pMap[i].tthorn == 0)fthorn = 0;
			if (pMap[i].tthorn == 1)fthorn = 0;
		}
	if (pHero->tthorn == 0 && pHero->ifthorn == 1) {
		step--;
		sblood = 1;
	}
}
int IfMob(int mstep) {
	int flag = 0;
	if ((pHero + mstep)->type == MOB)
	{
		TThorn();
		flag = 1;
		InitKickFx(mstep);
		fkick = 0;
		if ((pHero + mstep * 2)->type != ROAD || (pHero + mstep * 2) == pEnd)
		{
			InitKillMob(mstep);
		 PlaySound(_T("把怪物踢碎.wav"), NULL, SND_ASYNC);
		}
		else {
			PlaySound(_T("把怪物踢开.wav"), NULL, SND_ASYNC);
			fmob = 0;
			(pHero + mstep)->type = ROAD;
			(pHero + mstep * 2)->type = MOB;
			(pHero + mstep * 2)->mobifright = (pHero + mstep)->mobifright;
			switch (mstep)
			{
			case 1:
				(pHero + mstep * 2)->mobifright = 0;
				break;
			case -1:
				(pHero + mstep * 2)->mobifright = 1;
				break;
			}
		}
		step--;
	}
	return flag;
}
void Dead() {
	mciSendString(_T("close bkmusic"), NULL, 0, NULL);
	PlaySound(_T("人物死亡.wav"), NULL, SND_ASYNC);
	cleardevice();
	for (int i = 0; i < 18; i++) {
		putimage(pHero->x * 100 - 280, pHero->y * 100 - 800, a + 31 + i);
		FlushBatchDraw();
		Sleep(50);
		cleardevice();
	}
}
void DrawFrame() {
	putimage(0, 0, a + 24);
	DrawThorn();
	DrawHero();
	DrawBlood();
	DrawItem();
	DrawKickFx();
	DrawSkill();
	DrawStep();
	DrawDemon();
	DrawTime();
}
void Draw()
{
	while (1)
	{
		DrawFrame();
		FlushBatchDraw();
		cleardevice();
		if (_kbhit() && fmove == 6)break;
		if (f == 11.75)f = 0;
		else f += 0.25;
		Sleep(11.7);
	}
}
void KeyDown()
{
	c = _getch();
	switch (c)
	{
	case 'a':
	case 'A':
	case 75:
		ifright = 0;
		c = -1;
		if (IfMob(-1) != 0)break;
		if ((pHero - 1)->type == ROAD)
		{
			pHero--; fmove = 0; TThorn(); fdust = 0; tdust = rand() % 3; step--;
			if (pHero == pEnd)ifend = 1; 
			if (step == 0 || pHero == pEnd)pHero++; 
			else PlaySound(_T("人物移动.wav"), NULL, SND_ASYNC);
		}
		break;
	case 'd':
	case 'D':
	case 77:
		ifright = 1;
		c = 1;
		if (IfMob(1) != 0)break;
		if ((pHero + 1)->type == ROAD)
		{
			pHero++;  fmove = 0; TThorn(); fdust = 0; tdust = rand() % 3; step--;
			if (pHero == pEnd)ifend = 1;
			if (step == 0 || pHero == pEnd)pHero--; 
			else PlaySound(_T("人物移动.wav"), NULL, SND_ASYNC);
		}
		break;
	case 'w':
	case 'W':
	case 72:
		c = -MAPW;
		if (IfMob(-MAPW) != 0)break;
		if ((pHero - MAPW)->type == ROAD)
		{
			pHero -= MAPW;  fmove = 0; TThorn(); fdust = 0; tdust = rand() % 3; step--;
			if (pHero == pEnd)ifend = 1;
			if (step == 0 || pHero == pEnd)pHero += MAPW; 
			else PlaySound(_T("人物移动.wav"), NULL, SND_ASYNC);	
		}
		break;
	case 's':
	case 'S':
	case 80:
		c = MAPW;
		if (IfMob(MAPW) != 0)break;
		if ((pHero + MAPW)->type == ROAD)
		{
			pHero += MAPW;  fmove = 0; TThorn(); fdust = 0; tdust = rand() % 3; step--;
			if (pHero == pEnd)ifend = 1; 
			if (step == 0 || pHero == pEnd)pHero -= MAPW; 
			else PlaySound(_T("人物移动.wav"), NULL, SND_ASYNC);
		}
		break;
	case 'r':
	case 'R':
		if (skill > 0 && ((pHero - 1)->type == MOB || (pHero + 1)->type == MOB || (pHero - MAPW)->type == MOB || (pHero + MAPW)->type == MOB)) {
		PlaySound(_T("把怪物踢碎.wav"), NULL, SND_ASYNC);
			Skill();
			skill--;
			fskill = 0;
			fskillhero = 0;
		}
		break;
	}
	if (ifend == 1 || step == 0)return;
	Draw();
}
void getCountR(PGRID pMap, MYROAD* mRoad)
{
	int i, j;
	int k = 0;
	int count = 0;
	for (i = 0; i < MAPH; i++)
	{
		for (j = 0; j < MAPW; j++)
		{
			if (pMap[i * MAPW + j].type == ROAD)
			{
				mRoad->r[k] = i * MAPW + j;
				if (k > 0 && (mRoad->r[k] - mRoad->r[k - 1] == 9 || mRoad->r[k] - mRoad->r[k - 1] == 11))
				{
					count++;//由于键盘只能上下左右四个方向，所以如果计算机得出是对角线的结果则需要多加一步
				}
				k++;
				count++;
			}
		}
	}
	mRoad->length = k;
	mRoad->count = count;
}
//以上为高海峰的代码
//以下为葛耀文的代码
void putbkimg() {
	IMAGE bkimg;
	loadimage(&bkimg, _T("背景图.png"));
	putimage(0, 175, &bkimg);
}
void smile() {
	IMAGE img1, img2;
	putbkimg();
	loadimage(&img1, _T("idle.png"));
	loadimage(&img2, _T("LH_idle.png"));
	putimage(430, -50, &img1, SRCPAINT);
	putimage(430, -50, &img2, SRCAND);
	FlushBatchDraw();
}
void print(int z, int R, int G, int B, const wchar_t* Lsentence1, const wchar_t* Lsentence2)
{
	settextstyle(z, 0, _T("华康饰艺体W5"));
	settextcolor(RGB(R, G, B));
	RECT r10{ 0,550,1279,589 };
	RECT r20{ 0,600,1279,639 };
	drawtext(_T(sentence1), &r10, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext(_T(sentence2), &r20, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	FlushBatchDraw();
}
void print1(int z, int R, int G, int B, const wchar_t* Lsentence1, const wchar_t* Lsentence2)
{
	settextstyle(z, 0, _T("华文细黑"));
	settextcolor(RGB(R, G, B));
	RECT r10{ 0,550,1279,589 };
	RECT r20{ 0,600,1279,639 };
	drawtext(_T(sentence1), &r10, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext(_T(sentence2), &r20, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	FlushBatchDraw();
}
void loadpic1(const wchar_t* Lpic)
{
	IMAGE img;
	loadimage(&img, _T(pic));
	putimage(590, 620, &img);
	putbkimg();
	smile();
	print1(30, 210, 180, 192, _T("恭喜你通过了考验"), _T("你的妻子正在前面等你"));
	FlushBatchDraw();
	Sleep(30);
	cleardevice();

}
void loadpic2(const wchar_t* Lpic)
{
	IMAGE img;
	loadimage(&img, _T(pic));
	putimage(590, 620, &img);
	putbkimg();
	smile();
	print1(30, 210, 180, 192, _T("不过我有一个小要求"), _T("在回到人间之前，你不能看她"));
	FlushBatchDraw();
	Sleep(30);
	cleardevice();

}
void shake() {
	putbkimg();
	IMAGE img1, img2, img3, img4, img5, img6, img7, img8, img9, img10;
	loadimage(&img1, _T("swirl1.png"));
	loadimage(&img2, _T("Lswirl1.png"));
	putimage(430, -50, &img1, SRCPAINT);
	putimage(430, -50, &img2, SRCAND);
	FlushBatchDraw();
	Sleep(200);
	cleardevice();
	putbkimg();
	loadimage(&img3, _T("swirl2.png"));
	loadimage(&img4, _T("Lswirl2.png"));
	putimage(430, -50, &img3, SRCPAINT);
	putimage(430, -50, &img4, SRCAND);
	FlushBatchDraw();
	Sleep(200);
	cleardevice();
	putbkimg();
	loadimage(&img5, _T("swirl3.png"));
	loadimage(&img6, _T("Lswirl3.png"));
	putimage(430, -50, &img5, SRCPAINT);
	putimage(430, -50, &img6, SRCAND);
	FlushBatchDraw();
	Sleep(200);
	cleardevice();
	putbkimg();
	loadimage(&img7, _T("swirl4.png"));
	loadimage(&img8, _T("Lswirl4.png"));
	putimage(430, -50, &img7, SRCPAINT);
	putimage(430, -50, &img8, SRCAND);
	FlushBatchDraw();
	Sleep(200);
	cleardevice();
	putbkimg();
	loadimage(&img9, _T("swirl5.png"));
	loadimage(&img10, _T("Lswirl5.png"));
	putimage(430, -50, &img9, SRCPAINT);
	putimage(430, -50, &img10, SRCAND);
	FlushBatchDraw();
	Sleep(200);
}
void face() {
	IMAGE face1, face2, face3, face4, img1;
	loadimage(&img1, _T("swirl5.png"));
	loadimage(&face1, _T("Lswirl5.png"));
	putimage(430, -50, &img1, SRCPAINT);
	putimage(430, -50, &face1, SRCAND);
	FlushBatchDraw();
	Sleep(100);
	cleardevice();
	putbkimg();
	loadimage(&face2, _T("face2.png"));
	putimage(430, -50, &img1, SRCPAINT);
	putimage(430, -50, &face2, SRCAND);
	FlushBatchDraw();
	Sleep(100);
	cleardevice();
	putbkimg();
	loadimage(&face3, _T("face3.png"));
	putimage(430, -50, &img1, SRCPAINT);
	putimage(430, -50, &face3, SRCAND);
	FlushBatchDraw();
	Sleep(100);
	cleardevice();
	putbkimg();
	loadimage(&face4, _T("face4.png"));
	putimage(430, -50, &img1, SRCPAINT);
	putimage(430, -50, &face4, SRCAND);
	FlushBatchDraw();
	Sleep(500);
}
void loading1() {
	loadpic1(_T("booper0001.png"));
	loadpic1(_T("booper0002.png"));
	loadpic1(_T("booper0003.png"));
	loadpic1(_T("booper0004.png"));
	loadpic1(_T("booper0005.png"));
	loadpic1(_T("booper0006.png"));
	loadpic1(_T("booper0007.png"));
	loadpic1(_T("booper0008.png"));
	loadpic1(_T("booper0009.png"));
	loadpic1(_T("booper0010.png"));
	loadpic1(_T("booper0011.png"));
	loadpic1(_T("booper0012.png"));
	loadpic1(_T("booper0013.png"));
	loadpic1(_T("booper0014.png"));
	loadpic1(_T("booper0015.png"));
	loadpic1(_T("booper0016.png"));
	loadpic1(_T("booper0017.png"));

}
void loading2() {
	loadpic2(_T("booper0001.png"));
	loadpic2(_T("booper0002.png"));
	loadpic2(_T("booper0003.png"));
	loadpic2(_T("booper0004.png"));
	loadpic2(_T("booper0005.png"));
	loadpic2(_T("booper0006.png"));
	loadpic2(_T("booper0007.png"));
	loadpic2(_T("booper0008.png"));
	loadpic2(_T("booper0009.png"));
	loadpic2(_T("booper0010.png"));
	loadpic2(_T("booper0011.png"));
	loadpic2(_T("booper0012.png"));
	loadpic2(_T("booper0013.png"));
	loadpic2(_T("booper0014.png"));
	loadpic2(_T("booper0015.png"));
	loadpic2(_T("booper0016.png"));
	loadpic2(_T("booper0017.png"));
}
void loop1() {
	int i = 0;
	while (1) {
		loading1();
		if (_kbhit()) {
			i = _getch();
			if (i >= 32 && i <= 127)break;
		}
	}
}
void loop2() {
	int i = 0;
	while (1) {
		loading2();
		if (_kbhit()) {
			i = _getch();
			if (i >= 32 && i <= 127)break;
		}
	}
}
void end()
{
	loop1();
	cleardevice();
	putbkimg();
	smile();
	loop2();
	cleardevice();
	IMAGE ending;
	loadimage(&ending, _T("结局图.png"));
	putimage(0, 130, &ending);
	FlushBatchDraw();
	_getch();
}
//以上为葛耀文的代码
//以下为王天驹的代码
void introduce2()
{
	loadimage(&bkimg8, _T("背景图.png"));
	putimage(0, 175, &bkimg8);
	loadimage(&maskimg, _T("人2掩码.png"));
	putimage(400, -85, &maskimg, SRCAND);
	loadimage(&srcimg, _T("人2.png"));
	putimage(400, -85, &srcimg, SRCPAINT);
}

void introduce3()
{
	if (second == 0)
	{
		loadimage(&bkimg9, _T("未标题-2.png"));
		putimage(0, 175, &bkimg9);
	}
	else {
		loadimage(&bkimg10, _T("未标题-3.png"));
		putimage(0, 175, &bkimg10);
	}
	loadimage(&maskimg, _T("人3掩码.png"));
	putimage(400, -85, &maskimg, SRCAND);
	loadimage(&srcimg, _T("人3.png"));
	putimage(400, -85, &srcimg, SRCPAINT);
}

int communicate1(int x, const wchar_t* Lsentence1, const wchar_t* Lsentence2, const wchar_t* Lsentence3)
{
	settextstyle(40, 0, _T("Algerian"));                 //修改字体样式和字体大小
	cleardevice();

	IMAGE maskimg4, maskimg5, maskimg6;
	IMAGE srcimg4, srcimg5, srcimg6;
	settextcolor(RGB(210, 183, 189));
	loadimage(&maskimg4, _T("暗y1.png"));
	putimage(x, 585, &maskimg4, SRCAND);
	loadimage(&srcimg4, _T("暗1.png"));
	putimage(x, 585, &srcimg4, SRCPAINT);
	//introduce3();
	RECT r1 = { x, 530, x + 300, 710 };
	drawtext(_T(sentence1), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	loadimage(&maskimg5, _T("暗y1.png"));
	putimage(x + 400, 585, &maskimg5, SRCAND);
	loadimage(&srcimg5, _T("暗1.png"));
	putimage(x + 400, 585, &srcimg5, SRCPAINT);
	//introduce3();
	RECT r2 = { x + 400, 530,x + 700 , 710 };
	drawtext(_T(sentence2), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	loadimage(&maskimg6, _T("暗y1.png"));
	putimage(x + 800, 585, &maskimg6, SRCAND);
	loadimage(&srcimg6, _T("暗1.png"));
	putimage(x + 800, 585, &srcimg6, SRCPAINT);
	introduce3();
	RECT r3 = { x + 800, 585,x + 1100 , 710 };
	drawtext(_T(sentence3), &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	MOUSEMSG n;
	while (true)
	{
		n = GetMouseMsg();
		if ((n.x <= x + 300 && n.x >= x && n.y <= 665 && n.y >= 585))        //鼠标移动到某位置时方框变大
		{
			cleardevice();
			//IMAGE ......
			loadimage(&maskimg4, _T("高亮y1.png"));
			putimage(x, 585, &maskimg4, SRCAND);			//1亮色方框在画布上的位置
			loadimage(&srcimg4, _T("高亮1.png"));
			putimage(x, 585, &srcimg4, SRCPAINT);
			settextcolor(RGB(210, 183, 189));
			loadimage(&maskimg5, _T("暗y1.png"));
			putimage(x + 400, 585, &maskimg5, SRCAND);           //2暗色方框在画布上的位置
			loadimage(&srcimg5, _T("暗1.png"));
			putimage(x + 400, 585, &srcimg5, SRCPAINT);
			loadimage(&maskimg6, _T("暗y1.png"));
			putimage(x + 800, 585, &maskimg6, SRCAND);           //3暗色方框在画布上的位置
			loadimage(&srcimg6, _T("暗1.png"));
			putimage(x + 800, 585, &srcimg6, SRCPAINT);
			introduce3();
			RECT r3 = { x + 800, 530, x + 1100, 710 };
			drawtext(_T(sentence3), &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			RECT r2 = { x + 400, 530, x + 700, 710 };
			drawtext(_T(sentence2), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			settextcolor(WHITE);
			RECT r1 = { x , 530, x + 300, 710 };
			drawtext(_T(sentence1), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			FlushBatchDraw();
			if (n.uMsg == WM_LBUTTONDOWN)
			{
				k = 1;
				break;
			}
		}
		else if ((n.x <= x + 700 && n.x >= x && n.y <= 665 && n.y >= 585))        //鼠标移动到某位置时方框变大
		{
			cleardevice();
			//IMAGE ......
			loadimage(&maskimg5, _T("高亮y1.png"));
			putimage(x + 400, 585, &maskimg5, SRCAND);			//2亮色方框在画布上的位置
			loadimage(&srcimg5, _T("高亮1.png"));
			putimage(x + 400, 585, &srcimg5, SRCPAINT);
			settextcolor(RGB(210, 183, 189));
			loadimage(&maskimg4, _T("暗y1.png"));
			putimage(x, 585, &maskimg4, SRCAND);           //1暗色方框在画布上的位置
			loadimage(&srcimg4, _T("暗1.png"));
			putimage(x, 585, &srcimg4, SRCPAINT);
			loadimage(&maskimg6, _T("暗y1.png"));
			putimage(x + 800, 585, &maskimg6, SRCAND);           //3暗色方框在画布上的位置
			loadimage(&srcimg6, _T("暗1.png"));
			putimage(x + 800, 585, &srcimg6, SRCPAINT);
			introduce3();
			RECT r3 = { x + 800, 530, x + 1100, 710 };
			drawtext(_T(sentence3), &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			RECT r1 = { x , 530, x + 300, 710 };
			drawtext(_T(sentence1), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			settextcolor(WHITE);
			RECT r2 = { x + 400, 530, x + 700, 710 };
			drawtext(_T(sentence2), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			FlushBatchDraw();
			if (n.uMsg == WM_LBUTTONDOWN)
			{
				k = 2;
				break;
			}
		}
		else if ((n.x <= x + 1100 && n.x >= x + 800 && n.y <= 665 && n.y >= 585))        //鼠标移动到某位置时方框变大
		{
			settextstyle(40, 0, _T("Algerian"));
			cleardevice();
			//IMAGE ......
			loadimage(&maskimg6, _T("高亮y1.png"));
			putimage(x + 800, 585, &maskimg6, SRCAND);			//3亮色方框在画布上的位置
			loadimage(&srcimg6, _T("高亮1.png"));
			putimage(x + 800, 585, &srcimg6, SRCPAINT);
			settextcolor(RGB(210, 183, 189));
			loadimage(&maskimg5, _T("暗y1.png"));
			putimage(x + 400, 585, &maskimg5, SRCAND);           //2暗色方框在画布上的位置
			loadimage(&srcimg5, _T("暗1.png"));
			putimage(x + 400, 585, &srcimg5, SRCPAINT);
			loadimage(&maskimg4, _T("暗y1.png"));
			putimage(x, 585, &maskimg4, SRCAND);           //1暗色方框在画布上的位置
			loadimage(&srcimg4, _T("暗1.png"));
			putimage(x, 585, &srcimg4, SRCPAINT);
			introduce3();
			RECT r1 = { x , 530, x + 300, 710 };
			drawtext(_T(sentence1), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			RECT r2 = { x + 400, 530, x + 700, 710 };
			drawtext(_T(sentence2), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			settextcolor(WHITE);
			RECT r3 = { x + 800, 530, x + 1100, 710 };
			drawtext(_T(sentence3), &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			FlushBatchDraw();
			if (n.uMsg == WM_LBUTTONDOWN)
			{
				k = 3;
				break;
			}
		}
		else   //2暗色方框（与上面的一样
		{
			settextstyle(40, 0, _T("Algerian"));
			cleardevice();
			//IMAGE ......
			loadimage(&maskimg6, _T("暗y1.png"));
			putimage(x + 800, 585, &maskimg6, SRCAND);			//3暗色方框在画布上的位置
			loadimage(&srcimg6, _T("暗1.png"));
			putimage(x + 800, 585, &srcimg6, SRCPAINT);
			settextcolor(RGB(210, 183, 189));
			loadimage(&maskimg5, _T("暗y1.png"));
			putimage(x + 400, 585, &maskimg5, SRCAND);           //2暗色方框在画布上的位置
			loadimage(&srcimg5, _T("暗1.png"));
			putimage(x + 400, 585, &srcimg5, SRCPAINT);
			loadimage(&maskimg4, _T("暗y1.png"));
			putimage(x, 585, &maskimg4, SRCAND);           //1暗色方框在画布上的位置
			loadimage(&srcimg4, _T("暗1.png"));
			putimage(x, 585, &srcimg4, SRCPAINT);
			introduce3();
			RECT r1 = { x , 530, x + 300, 710 };
			drawtext(_T(sentence1), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			RECT r2 = { x + 400, 530, x + 700, 710 };
			drawtext(_T(sentence2), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			RECT r3 = { x + 800, 530, x + 1100, 710 };
			drawtext(_T(sentence3), &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			FlushBatchDraw();
		}
	}
	return 0;
}

int communicate(int y1, int y2, const wchar_t* Lsentence1, const wchar_t* Lsentence2)
{
	settextstyle(40, 0, _T("Algerian"));                 //修改字体样式和字体大小
	cleardevice();
	IMAGE maskimg1, maskimg2;
	IMAGE srcimg1, srcimg2;
	settextcolor(RGB(210, 183, 189));
	loadimage(&maskimg1, _T("暗y.png"));
	putimage(180, y1, &maskimg1, SRCAND);
	loadimage(&srcimg1, _T("暗.png"));
	putimage(180, y1, &srcimg1, SRCPAINT);
	introduce2();
	RECT r1 = { 0, y1, 1280, y1 + 80 };
	drawtext(_T(sentence1), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	settextcolor(RGB(210, 183, 189));
	loadimage(&maskimg2, _T("暗y.png"));
	putimage(180, y2, &maskimg2, SRCAND);
	loadimage(&srcimg2, _T("暗.png"));
	putimage(180, y2, &srcimg2, SRCPAINT);
	introduce2();
	RECT r2 = { 0, y2, 1280, y2 + 80 };
	drawtext(_T(sentence2), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	MOUSEMSG m;

	while (true)
	{
		m = GetMouseMsg();
		if ((m.x <= 1100 && m.x >= 150 && m.y <= y1 + 90 && m.y >= y1))        //鼠标移动到某位置时方框变大
		{
			settextstyle(40, 0, _T("Algerian"));
			cleardevice();
			IMAGE maskimg10;
			IMAGE srcimg10;
			loadimage(&maskimg10, _T("高亮y.png"));
			putimage(150, y1, &maskimg10, SRCAND);			//1亮色方框在画布上的位置
			loadimage(&srcimg10, _T("高亮.png"));
			putimage(150, y1, &srcimg10, SRCPAINT);
			settextcolor(RGB(210, 183, 189));
			loadimage(&maskimg2, _T("暗y.png"));
			putimage(180, y2, &maskimg2, SRCAND);           //2暗色方框在画布上的位置
			loadimage(&srcimg2, _T("暗.png"));
			putimage(180, y2, &srcimg2, SRCPAINT);
			introduce2();
			RECT r2 = { 0, y2, 1280, y2 + 80 };
			drawtext(_T(sentence2), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			settextcolor(WHITE);
			RECT r1 = { 0, y1, 1280, y1 + 80 };
			drawtext(_T(sentence1), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				j = 1;
				break;
			}
		}
		else   //1暗色方框（与上面的一样
		{
			cleardevice();
			settextcolor(RGB(210, 183, 189));
			loadimage(&maskimg1, _T("暗y.png"));
			putimage(180, y1, &maskimg1, SRCAND);
			loadimage(&srcimg1, _T("暗.png"));
			putimage(180, y1, &srcimg1, SRCPAINT);
			introduce2();
			RECT r1 = { 0, y1, 1280, y1 + 80 };
			drawtext(_T(sentence1), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			if ((m.x <= 1100 && m.x >= 150 && m.y <= y2 + 90 && m.y >= y2))        //鼠标移动到某位置时方框变大
			{
				settextcolor(WHITE);
				IMAGE maskimg20;
				IMAGE srcimg20;
				loadimage(&maskimg20, _T("高亮y.png"));
				putimage(150, y2, &maskimg20, SRCAND);			//2亮色方框在画布上的位置
				loadimage(&srcimg20, _T("高亮.png"));
				putimage(150, y2, &srcimg20, SRCPAINT);
				RECT r2 = { 0, y2, 1280, y2 + 80 };
				drawtext(_T(sentence2), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				FlushBatchDraw();
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					j = 2;
					break;
				}
			}
			else   //2暗色方框（与上面的一样
			{
				cleardevice();
				settextcolor(RGB(210, 183, 189));
				loadimage(&maskimg1, _T("暗y.png"));
				putimage(180, y1, &maskimg1, SRCAND);
				loadimage(&srcimg1, _T("暗.png"));
				putimage(180, y1, &srcimg1, SRCPAINT);
				introduce2();
				RECT r1 = { 0, y1, 1280, y1 + 80 };
				drawtext(_T(sentence1), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				settextcolor(RGB(210, 183, 189));
				loadimage(&maskimg2, _T("暗y.png"));
				putimage(180, y2, &maskimg2, SRCAND);
				loadimage(&srcimg2, _T("暗.png"));
				putimage(180, y2, &srcimg2, SRCPAINT);
				introduce2();
				RECT r2 = { 0, y2, 1280, y2 + 80 };
				drawtext(_T(sentence2), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			FlushBatchDraw();
		}
	}

	return 0;
}

void introduce1()
{
	loadimage(&bkimg7, _T("背景图.png"));
	putimage(0, 175, &bkimg7);
	loadimage(&maskimg, _T("人1掩码.png"));
	putimage(400, -85, &maskimg, SRCAND);
	loadimage(&srcimg, _T("人1.png"));
	putimage(400, -85, &srcimg, SRCPAINT);
}

void changing1()
{

	loadimage(&bkimg1, _T("tiny.png"));
	putimage(500, 243, &bkimg1);
	print(4, 35, 30, 32, _T("某天你从梦中惊醒"), _T("你梦到你的妻子欧律狄克被冥王带到了地狱"));
	loadimage(&bkimg2, _T("small.png"));
	putimage(426, 194, &bkimg2);
	print(8, 70, 61, 63, _T("某天你从梦中惊醒"), _T("你梦到你的妻子欧律狄克被冥王带到了地府"));
	Sleep(20);
	cleardevice();
	loadimage(&bkimg3, _T("medium.png"));
	putimage(314, 124, &bkimg3);
	print(16, 105, 91, 95, _T("某天你从梦中惊醒"), _T("你梦到你的妻子欧律狄克被冥王带到了地狱"));
	Sleep(20);
	cleardevice();
	loadimage(&bkimg4, _T("big.png"));
	putimage(200, 67, &bkimg4);
	print(24, 140, 122, 126, _T("某天你从梦中惊醒"), _T("你梦到你的妻子欧律狄克被冥王带到了地狱"));
	Sleep(20);
	cleardevice();
	loadimage(&bkimg5, _T("shrink.png"));
	putimage(205, 59, &bkimg5);
	print(32, 175, 153, 158, _T("某天你从梦中惊醒"), _T("你梦到你的妻子欧律狄克被冥王带到了地狱"));
	FlushBatchDraw();
	Sleep(20);
	cleardevice();
	loadimage(&bkimg6, _T("last.png"));
	putimage(210, 64, &bkimg6);
	print(40, 210, 183, 189, _T("某天你从梦中惊醒"), _T("你梦到你的妻子欧律狄克被冥王带到了地狱"));
	loadimage(&bkimg, _T("1'.png"));
	putimage(220, 100, &bkimg);
	FlushBatchDraw();
	_getch();
	cleardevice();
	//第二页
	//
	loadimage(&bkimg1, _T("tiny.png"));
	putimage(500, 243, &bkimg1);
	print(4, 35, 30, 32, _T("你下定决心，要把妻子救回来"), _T("虽然擅闯地狱并不是明智的选择"));
	FlushBatchDraw();
	Sleep(20);
	cleardevice();
	loadimage(&bkimg2, _T("small.png"));
	putimage(426, 194, &bkimg2);
	print(8, 70, 61, 63, _T("你下定决心，要把妻子救回来"), _T("虽然擅闯地狱并不是明智的选择"));
	FlushBatchDraw();
	Sleep(20);
	cleardevice();
	loadimage(&bkimg3, _T("medium.png"));
	putimage(314, 124, &bkimg3);
	print(16, 105, 91, 95, _T("你下定决心，要把妻子救回来"), _T("虽然擅闯地狱并不是明智的选择"));
	Sleep(20);
	cleardevice();
	loadimage(&bkimg4, _T("big.png"));
	putimage(200, 67, &bkimg4);
	print(24, 140, 122, 126, _T("你下定决心，要把妻子救回来"), _T("虽然擅闯地狱并不是明智的选择"));
	FlushBatchDraw();
	Sleep(20);
	cleardevice();
	loadimage(&bkimg5, _T("shrink.png"));
	putimage(205, 59, &bkimg5);
	print(32, 175, 153, 158, _T("你下定决心，要把妻子救回来"), _T("虽然擅闯地狱并不是明智的选择"));
	FlushBatchDraw();
	Sleep(20);
	cleardevice();
	loadimage(&bkimg6, _T("last.png"));
	putimage(210, 64, &bkimg6);
	print(40, 210, 183, 189, _T("你下定决心，要把妻子救回来"), _T("虽然擅闯地狱并不是明智的选择"));
	loadimage(&bkimg, _T("2'.png"));
	putimage(220, 100, &bkimg);
	FlushBatchDraw();
	_getch();
	cleardevice();
	//第三页
	loadimage(&bkimg1, _T("tiny.png"));
	putimage(500, 243, &bkimg1);
	print(4, 35, 30, 32, _T("“无论如何，我会把你带回来的”"), _T("于是你踏上了去往地狱的漫漫长路"));
	FlushBatchDraw();
	Sleep(20);
	cleardevice();
	loadimage(&bkimg2, _T("small.png"));
	putimage(426, 194, &bkimg2);
	print(8, 70, 61, 63, _T("“无论如何，我会把你带回来的”"), _T("于是你踏上了去往地狱的漫漫长路"));
	FlushBatchDraw();
	Sleep(20);
	cleardevice();
	loadimage(&bkimg3, _T("medium.png"));
	putimage(314, 124, &bkimg3);
	print(16, 105, 91, 95, _T("“无论如何，我会把你带回来的”"), _T("于是你踏上了去往地狱的漫漫长路"));
	FlushBatchDraw();
	Sleep(20);
	cleardevice();
	loadimage(&bkimg4, _T("big.png"));
	putimage(200, 67, &bkimg4);
	print(24, 140, 122, 126, _T("“无论如何，我会把你带回来的”"), _T("于是你踏上了去往地狱的漫漫长路"));
	FlushBatchDraw();
	Sleep(20);
	cleardevice();
	loadimage(&bkimg5, _T("shrink.png"));
	putimage(205, 59, &bkimg5);
	print(32, 175, 153, 158, _T("“无论如何，我会把你带回来的”"), _T("于是你踏上了去往地狱的漫漫长路"));
	FlushBatchDraw();
	Sleep(20);
	cleardevice();
	loadimage(&bkimg6, _T("last.png"));
	putimage(210, 64, &bkimg6);
	print(40, 210, 183, 189, _T("“无论如何，我会把你带回来的”"), _T("于是你踏上了去往地狱的漫漫长路"));
	loadimage(&bkimg, _T("3'.png"));
	putimage(220, 100, &bkimg);
	FlushBatchDraw();
	//
	_getch();
	for (int y = -720; y <= 0; y = y + 60)
	{
		loadimage(&bkimg6, _T("last.png"));
		putimage(210, 64, &bkimg6);
		settextstyle(40, 0, _T("华康饰艺体W5"));
		settextcolor(RGB(210, 183, 189));
		RECT r10{ 0,550,1279,589 };
		RECT r20{ 0,600,1279,639 };
		drawtext(_T("“无论如何，我会把你带回来的”"), &r10, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("于是你踏上了去往地狱的漫漫长路"), &r20, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		loadimage(&bkimg, _T("3'.png"));
		putimage(220, 100, &bkimg);
		setfillcolor(RGB(2, 2, 27));
		setlinecolor(RGB(2, 2, 27));
		fillrectangle(0, y, 1280, y + 720);
		FlushBatchDraw();
		Sleep(10);
	}
	cleardevice();
	//第四页
	for (int i = -1280; i <= 0; i = i + 80)
	{
		loadimage(&bkimg7, _T("背景图.png"));
		putimage(i, 175, &bkimg7);
		loadimage(&maskimg, _T("人1掩码.png"));
		putimage(400 - i, -85, &maskimg, SRCAND);
		loadimage(&srcimg, _T("人1.png"));
		putimage(400 - i, -85, &srcimg, SRCPAINT);
		FlushBatchDraw();
		Sleep(10);
		cleardevice();
	}
	//
	introduce1();
	print1(4, 35, 30, 32, _T("我是冥河摆渡人"), _T("过了这条河就是地狱"));
	FlushBatchDraw();
	Sleep(20);
	cleardevice();
	introduce1();
	print1(8, 70, 61, 63, _T("我是冥河摆渡人"), _T("过了这条河就是地狱"));
	FlushBatchDraw();
	Sleep(20);
	cleardevice();
	introduce1();
	print1(16, 105, 91, 95, _T("我是冥河摆渡人"), _T("过了这条河就是地狱"));
	FlushBatchDraw();
	Sleep(20);
	cleardevice();
	introduce1();
	print1(24, 140, 122, 126, _T("我是冥河摆渡人"), _T("过了这条河就是地狱"));
	FlushBatchDraw();
	Sleep(20);
	cleardevice();
	introduce1();
	print1(32, 175, 152, 158, _T("我是冥河摆渡人"), _T("过了这条河就是地狱"));
	FlushBatchDraw();
	Sleep(20);
	cleardevice();
	introduce1();
	print1(40, 210, 183, 189, _T("我是冥河摆渡人"), _T("过了这条河就是地狱"));
	FlushBatchDraw();
	_getch();
	cleardevice();
	//第五页 
	introduce2();
	print1(4, 35, 30, 32, _T("前方危险重重"), _T("希望你做好了准备"));
	FlushBatchDraw();
	Sleep(20);
	cleardevice();
	introduce2();
	print1(8, 70, 61, 63, _T("前方危险重重"), _T("希望你做好了准备"));
	FlushBatchDraw();
	Sleep(20);
	cleardevice();
	introduce2();
	print1(16, 105, 91, 95, _T("前方危险重重"), _T("希望你做好了准备"));
	FlushBatchDraw();
	Sleep(20);
	cleardevice();
	introduce2();
	print1(24, 140, 122, 126, _T("前方危险重重"), _T("希望你做好了准备"));
	FlushBatchDraw();
	Sleep(20);
	cleardevice();
	introduce2();
	print1(32, 175, 152, 158, _T("前方危险重重"), _T("希望你做好了准备"));
	FlushBatchDraw();
	Sleep(20);
	cleardevice();
	introduce2();
	print1(40, 210, 183, 189, _T("前方危险重重"), _T("希望你做好了准备"));
	_getch();
}
void changing2()
{
	communicate(550, 610, _T("Start Game"), _T("Quit"));
	cleardevice();
	if (j == 1)
		{
			introduce3();
			communicate1(50, _T("Easy"), _T("Normal"), _T("Hard"));
		}
}
void introduce5()
{
	IMAGE img1, img2;
	putbkimg();
	loadimage(&img1, _T("idle.png"));
	loadimage(&img2, _T("LH_idle.png"));
	putimage(430, -50, &img1, SRCPAINT);
	putimage(430, -50, &img2, SRCAND);
}

void introduce6()
{
	IMAGE bkimg;
	IMAGE maskimg;
	IMAGE srcimg;
	loadimage(&bkimg, _T("背景图.png"));
	putimage(0, 175, &bkimg);
	loadimage(&maskimg, _T("LH_angry1.png"));
	putimage(400, -100, &maskimg, SRCAND);
	loadimage(&srcimg, _T("angry1.png"));
	putimage(400, -100, &srcimg, SRCPAINT);
}
void death()
{
	PlaySound(_T("bad end.wav"), NULL, SND_ASYNC);
	IMAGE i1, i2, i3, i4, i5, i6, i7, i8, i9;
	loadimage(&i1, _T("d1.jpg"));
	putimage(0, 164, &i1);
	FlushBatchDraw();
	Sleep(50);
	cleardevice();
	loadimage(&i2, _T("d2.jpg"));
	putimage(0, 164, &i2);
	FlushBatchDraw();
	Sleep(50);
	cleardevice();
	loadimage(&i3, _T("d3.jpg"));
	putimage(0, 164, &i3);
	FlushBatchDraw();
	Sleep(50);
	cleardevice();
	loadimage(&i4, _T("d4.jpg"));
	putimage(0, 164, &i4);
	FlushBatchDraw();
	Sleep(50);
	cleardevice();
	loadimage(&i5, _T("d5.jpg"));
	putimage(0, 164, &i5);
	FlushBatchDraw();
	Sleep(50);
	cleardevice();
	loadimage(&i6, _T("d6.jpg"));
	putimage(0, 164, &i6);
	FlushBatchDraw();
	Sleep(50);
	cleardevice();
	loadimage(&i7, _T("d7.jpg"));
	putimage(0, 164, &i7);
	FlushBatchDraw();
	Sleep(50);
	cleardevice();
	loadimage(&i8, _T("d8.jpg"));
	putimage(0, 164, &i8);
	FlushBatchDraw();
	Sleep(50);
	cleardevice();
	loadimage(&i9, _T("d9.jpg"));
	putimage(0, 164, &i9);
	settextstyle(20, 0, _T("华康饰艺体W5"));
	settextcolor(RGB(205, 20, 16));
	RECT r30{ 0,650,1279,689 };
	drawtext(_T("她笑了笑，在你的额头上吻了一下，你的生命就此消融"), &r30, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	FlushBatchDraw();
	_getch();
}

int communicate3(int y1, int y2, const wchar_t* Lsentence1, const wchar_t* Lsentence2)
{
	settextstyle(40, 0, _T("Algerian"));                 //修改字体样式和字体大小
	cleardevice();
	IMAGE maskimg1, maskimg2;
	IMAGE srcimg1, srcimg2;
	settextcolor(RGB(210, 183, 189));
	loadimage(&maskimg1, _T("暗y.png"));
	putimage(180, y1, &maskimg1, SRCAND);
	loadimage(&srcimg1, _T("暗.png"));
	putimage(180, y1, &srcimg1, SRCPAINT);
	introduce6();
	RECT r1 = { 0, y1, 1280, y1 + 80 };
	drawtext(_T(sentence1), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	settextcolor(RGB(210, 183, 189));
	loadimage(&maskimg2, _T("暗y.png"));
	putimage(180, y2, &maskimg2, SRCAND);
	loadimage(&srcimg2, _T("暗.png"));
	putimage(180, y2, &srcimg2, SRCPAINT);
	introduce6();
	RECT r2 = { 0, y2, 1280, y2 + 80 };
	drawtext(_T(sentence2), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	MOUSEMSG m;
	while (true)
	{
		t12 = clock();
		if ((t12 - t11) > 5000) {
			j = 2; break;
		}
		m = GetMouseMsg();
		if ((m.x <= 1100 && m.x >= 150 && m.y <= y1 + 90 && m.y >= y1))        //鼠标移动到某位置时方框变大
		{
			settextstyle(40, 0, _T("Algerian"));
			cleardevice();
			IMAGE maskimg10;
			IMAGE srcimg10;
			loadimage(&maskimg10, _T("高亮y.png"));
			putimage(150, y1, &maskimg10, SRCAND);			//1亮色方框在画布上的位置
			loadimage(&srcimg10, _T("高亮.png"));
			putimage(150, y1, &srcimg10, SRCPAINT);
			settextcolor(RGB(210, 183, 189));
			loadimage(&maskimg2, _T("暗y.png"));
			putimage(180, y2, &maskimg2, SRCAND);           //2暗色方框在画布上的位置
			loadimage(&srcimg2, _T("暗.png"));
			putimage(180, y2, &srcimg2, SRCPAINT);
			introduce6();
			RECT r2 = { 0, y2, 1280, y2 + 80 };
			drawtext(_T(sentence2), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			settextcolor(WHITE);
			RECT r1 = { 0, y1, 1280, y1 + 80 };
			drawtext(_T(sentence1), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				j = 1;
				break;
			}
		}
		else  if ((m.x <= 1100 && m.x >= 150 && m.y <= y2 + 90 && m.y >= y2))        //鼠标移动到某位置时方框变大
		{
			cleardevice();
			settextcolor(RGB(210, 183, 189));
			loadimage(&maskimg1, _T("暗y.png"));
			putimage(180, y1, &maskimg1, SRCAND);
			loadimage(&srcimg1, _T("暗.png"));
			putimage(180, y1, &srcimg1, SRCPAINT);
			introduce6();
			RECT r1 = { 0, y1, 1280, y1 + 80 };
			drawtext(_T(sentence1), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			settextcolor(WHITE);
			IMAGE maskimg20;
			IMAGE srcimg20;
			loadimage(&maskimg20, _T("高亮y.png"));
			putimage(150, y2, &maskimg20, SRCAND);			//2亮色方框在画布上的位置
			loadimage(&srcimg20, _T("高亮.png"));
			putimage(150, y2, &srcimg20, SRCPAINT);
			RECT r2 = { 0, y2, 1280, y2 + 80 };
			drawtext(_T(sentence2), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			FlushBatchDraw();
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				j = 2;
				break;
			}
		}
		else   //2暗色方框（与上面的一样
		{
			cleardevice();
			settextcolor(RGB(210, 183, 189));
			loadimage(&maskimg1, _T("暗y.png"));
			putimage(180, y1, &maskimg1, SRCAND);
			loadimage(&srcimg1, _T("暗.png"));
			putimage(180, y1, &srcimg1, SRCPAINT);

			RECT r1 = { 0, y1, 1280, y1 + 80 };
			drawtext(_T(sentence1), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			settextcolor(RGB(210, 183, 189));
			loadimage(&maskimg2, _T("暗y.png"));
			putimage(180, y2, &maskimg2, SRCAND);
			loadimage(&srcimg2, _T("暗.png"));
			putimage(180, y2, &srcimg2, SRCPAINT);
			introduce6();
			RECT r2 = { 0, y2, 1280, y2 + 80 };
			drawtext(_T(sentence2), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		FlushBatchDraw();
	}
	return 0;
}

void asking()
{
	communicate3(550, 610, _T("我来寻找我的妻子"), _T("你这身衣服不错"));
}


void glorious()
{
	PlaySound(_T("success.wav"), NULL, SND_ASYNC);
	IMAGE j09, j10, j11, j12, j13, j14, j15, j16;
	introduce5();
	loadimage(&j09, _T("success0009.jpg"));
	putimage(195, 530, &j09);
	FlushBatchDraw();
	Sleep(50);
	cleardevice();
	introduce5();
	loadimage(&j10, _T("success0010.jpg"));
	putimage(195, 530, &j10);
	FlushBatchDraw();
	Sleep(50);
	cleardevice();
	introduce5();
	loadimage(&j11, _T("success0011.jpg"));
	putimage(195, 530, &j11);
	FlushBatchDraw();
	Sleep(50);
	cleardevice();
	introduce5();
	loadimage(&j12, _T("success0012.jpg"));
	putimage(195, 530, &j12);
	FlushBatchDraw();
	Sleep(50);
	cleardevice();
	introduce5();
	loadimage(&j13, _T("success0013.jpg"));
	putimage(195, 530, &j13);
	FlushBatchDraw();
	Sleep(50);
	cleardevice();
	introduce5();
	loadimage(&j14, _T("success0014.jpg"));
	putimage(195, 530, &j14);
	FlushBatchDraw();
	Sleep(50);
	cleardevice();
	introduce5();
	loadimage(&j15, _T("success0015.jpg"));
	putimage(195, 530, &j15);
	FlushBatchDraw();
	Sleep(50);
	cleardevice();
	introduce5();
	loadimage(&j16, _T("success0016.jpg"));
	putimage(195, 530, &j16);
	FlushBatchDraw();
	_getch();
}
//以上为王天驹的代码
void Draw1() {
	putimage(pHero->x * 100 - 280, pHero->y * 100 - 800, a + 31 + 17);
}
void Draw2() {
	int y1 = 550; int y2 = 610; const wchar_t* Lsentence1 = _T("Start Game"); const wchar_t* Lsentence2= _T("Quit");
		settextstyle(40, 0, _T("Algerian"));                 //修改字体样式和字体大小
		cleardevice();
		IMAGE maskimg1, maskimg2;
		IMAGE srcimg1, srcimg2;
		settextcolor(RGB(210, 183, 189));
		loadimage(&maskimg1, _T("暗y.png"));
		putimage(180, y1, &maskimg1, SRCAND);
		loadimage(&srcimg1, _T("暗.png"));
		putimage(180, y1, &srcimg1, SRCPAINT);
		introduce2();
		RECT r1 = { 0, y1, 1280, y1 + 80 };
		drawtext(_T(sentence1), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		settextcolor(RGB(210, 183, 189));
		loadimage(&maskimg2, _T("暗y.png"));
		putimage(180, y2, &maskimg2, SRCAND);
		loadimage(&srcimg2, _T("暗.png"));
		putimage(180, y2, &srcimg2, SRCPAINT);
		introduce2();
		RECT r2 = { 0, y2, 1280, y2 + 80 };
		drawtext(_T(sentence2), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
void Draw3() {
	IMAGE i9;
	loadimage(&i9, _T("d9.jpg"));
	putimage(0, 164, &i9);
	settextstyle(20, 0, _T("华康饰艺体W5"));
	settextcolor(RGB(205, 20, 16));
	RECT r30{ 0,650,1279,689 };
	drawtext(_T("她笑了笑，在你的额头上吻了一下，你的生命就此消融"), &r30, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
void Draw4() {
	putbkimg();
	IMAGE img1,img2;
	loadimage(&img1, _T("swirl1.png"));
	loadimage(&img2, _T("Lswirl1.png"));
	putimage(430, -50, &img1, SRCPAINT);
	putimage(430, -50, &img2, SRCAND);
}
void Draw5() {
	IMAGE ending;
	loadimage(&ending, _T("结局图.png"));
	putimage(0, 130, &ending);
}
int main()
{
	srand(unsigned(time(NULL)));
	initgraph(1280, 720);
	setbkcolor(RGB(2, 2, 27));
	setbkmode(TRANSPARENT);
	cleardevice();
	BeginBatchDraw();
	changing1();
	pos_1:
	changing2();
	if (j == 2) { 
		Transition1(Draw2);
		EndBatchDraw();
		closegraph();
		return 0;
	}
	switch (k) {
	case 1:
		W_MAX = rand() % 5+15;
		M_MAX = rand() % 5;
		T_MAX = rand() % 5;
		break;
	case 2:
		W_MAX = rand() % 5 + 10;
		M_MAX = rand() % 5+5;
		T_MAX = rand() % 5+5;
		break;
	case 3:
		W_MAX = rand() % 5 + 10;
		M_MAX = rand() % 3 + 7;
		T_MAX = rand() % 3+7;
		break;
	}
	timediff = 0;
	fmove = 6;  ifend = 0; skill = 3; ifright = 1; tmob = 0; sblood = 0;
	f = 0; fkick = 7; fmob = 6; fskill = 9; fskillhero = 6; fthorn = 4; fblood = 6;
	Transition1(introduce3);
	InitResource();
	InitMap();
	creat();
	while (getRoad(pMap, pHero, pEnd) != true)
	{
		InitMap();
		creat();
	}
	MYROAD mRoad;
	getCountR(pMap, &mRoad);
	creatRoad();
	creatMob();
	creatThorn();
	step = mRoad.count + 10 + rand() % 5;
	second = 1;
	Transition2(DrawFrame);
	tstart = clock();
	mciSendString(_T("open 背景音乐.mp3 alias bkmusic"), NULL, 0, NULL);
	mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);
	Draw();
	while (1)
	{
		KeyDown();
		if (ifend == 1 || (step <= 0))break;
	}
	mciSendString(_T("close bkmusic"), NULL, 0, NULL);
	if (ifend == 1) {
		fmove = 6;
		Transition1(DrawFrame);
		Transition2(Draw4);
		shake();
		cleardevice();
		shake();
		cleardevice();
		putbkimg();
		face();
		smile();
		cleardevice();
		t11 = clock();
		asking();
		if (j == 2)
		{
			death();
			Transition1(Draw3);
			Transition2(Draw2);
			goto pos_1;
		}
		else
		{
			glorious();
			end();
			Transition1(Draw5);
			EndBatchDraw();
			closegraph();
			return 0;
		}
	}
	else{
		Dead(); 
		Transition1(Draw1);
		Transition2(Draw2);
		goto pos_1;
	}
}
#endif