#include<Windows.h>
#include"Plate.h"
#define TIMER_ID 12345
//#define TIMER_ID 12346
#define IS_SPEEDUP(s)(s%dwOneLevelScores)==0

/************** 全局变量 ****************/
// 计时器到时间隔
DWORD dwTimerElapse;
// 每提高一级，计时器时钟周期缩短的比例。
DOUBLE dbLevelSpeedupRatio;
// 此变量表示多少个积分上升一个级别
DWORD dwOneLevelScores;
GAME_COORD ball;
// 保存计分，初始为0
int score = 0;
// 保存级别，初始为0
int level = 0;

/************** 函数定义 ****************/
// 返回积分
int GetScore() { return score; }

// 返回level
int GetLevel() { return level; }

// 提高速度。
BOOL SpeedUp(HWND hwnd)
{
	// 调整计时器到时时间
	dwTimerElapse = (DWORD)(dwTimerElapse * dbLevelSpeedupRatio);
	KillTimer(hwnd, TIMER_ID);
	SetTimer(hwnd, TIMER_ID, dwTimerElapse, NULL);
	return TRUE;
}
// 游戏控制的一个主要流程。
// 当计时器发生时进行的处理逻辑。
void OnTimer(HWND hwnd)
{
	//计时器到时以后，小球下落一步
	ball.y += 1;
	
	switch (PlateMove())
	{
	case PLATE_BROKEN:
		KillTimer(hwnd, TIMER_ID);
		DistroyPlate();
		MessageBox(0, "Game Over", "GAME OVER", 0);
		ExitProcess(0);
		break;
	case PLATE_GOTTEN_BALL:
		score++;
		//DistroyBall();
		CreateBall();
		if (IS_SPEEDUP(score))
		{
			level++;
			SpeedUp(hwnd);
		}
		if (GetLevel() >= 5)
		{
			KillTimer(hwnd, TIMER_ID);
			MessageBox(0, "YOU WIN", "YOU WIN", 0);
			ExitProcess(0);
		}
		break;
	case PLATE_MISSED_BALL:
		//判断是否降级,计算新的等级
		//DistroyBall();
		CreateBall();
		score = score - 2;
		level = score / 5;
		//slowdown()
		break;
	case PLATE_MOVED:
		break;
	}
	return;
}
void OnKeyDown(DWORD vk,HWND hwnd)
{
	switch (vk)//virtual key value
	{
	case VK_LEFT:
		SetDirection(PLATE_LEFT);
			break;
	case VK_RIGHT:
		SetDirection(PLATE_RIGHT);
		break;
	}
	//CreateBall();
	return;
}
// 游戏的初始化，
// 创建游戏的内部数据结构和系统对象。
void CreateGame(HWND hwnd, // 主窗口句柄
	DWORD dwInitTimerElapse, //
	unsigned int one_level_scores,
	DOUBLE level_speedup_ratio,
	int boundary_x, int boundary_y,
	int init_x, int init_y,
	int lenth)
{
	// 设置随机数种子
	// 需要使用随机数生成小球的位置等。
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	srand(ft.dwLowDateTime);

	dbLevelSpeedupRatio = level_speedup_ratio;
	dwTimerElapse = dwInitTimerElapse;
	dwOneLevelScores = one_level_scores;

	// 设置游戏的边界
	SetBoundary(boundary_x, boundary_y);
	// 创建表示plate的数据结构
	CreatePlate(init_x,init_y, lenth);
	// 创建表示ball的数据结构
	CreateBall();
	SetTimer(hwnd, TIMER_ID, dwTimerElapse, NULL);
}