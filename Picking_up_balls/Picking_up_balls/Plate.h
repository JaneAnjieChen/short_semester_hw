#include<Windows.h>
#include"LinkList'.h"
//定义盘子的状态
#define PLATE_MOVED 0
#define PLATE_BROKEN 1
#define PLATE_GOTTEN_BALL 2
#define PLATE_MISSED_BALL 3
#define PLATE_ERROR 4

//盘子的方向,枚举类型
typedef enum _direction
{
	PLATE_RIGHT,
	PLATE_LEFT,
}direction;

//坐标，coordinate
typedef struct _GAME_COORD
{
	short x;
	short y;
}GAME_COORD, *PGAME_COORD;

//函数声明

///Model_plate_and_balls.c中的接口函数

//设置边界坐标
void SetBoundary(int x, int y);

//获得边界坐标
PGAME_COORD GetBoundary();

//生成新的小球
int CreateBall();

//获得小球坐标
PGAME_COORD GetBall();

//创建盘子
void CreatePlate(int head_x, int head_y,int lenth);

//销毁盘子，释放内存
void DistroyPlate();

//销毁小球，释放内存
void DistoryBall();

//用来移动一步；如果接到小球，分数加1
int PlateMove();

//获得盘子第一个节点的坐标
PGAME_COORD GetPlateHead();

//获得盘子最后一个节点的坐标
PGAME_COORD GetSnakeTail();

//按照序号获得盘子的结点和坐标，不能超过盘子长度，否则返回NULL
PGAME_COORD GetPlateAt(int n);

//改变盘子移动的方向
void SetDirection(direction dir);

//获得当前盘子的方向
direction GetDirection();

///Controller_game_operation.c中的接口函数

int GetScore();
int GetLevel();
void CreateGame(HWND hwnd,DWORD dwInitTimerElapse,
	unsigned int one_level_scores,DOUBLE level_speedup_radio,
	int boundary_x,int boundary_y,
	int init_x,int init_y,int lenth);
void OnTimer(HWND hwnd);
void OnKeyDown(DWORD vk,HWND hwnd);

void SetDirection(direction dir);


