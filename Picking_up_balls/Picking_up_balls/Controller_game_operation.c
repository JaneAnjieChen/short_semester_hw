#include<Windows.h>
#include"Plate.h"
#define TIMER_ID 12345
//#define TIMER_ID 12346
#define IS_SPEEDUP(s)(s%dwOneLevelScores)==0

/************** ȫ�ֱ��� ****************/
// ��ʱ����ʱ���
DWORD dwTimerElapse;
// ÿ���һ������ʱ��ʱ���������̵ı�����
DOUBLE dbLevelSpeedupRatio;
// �˱�����ʾ���ٸ���������һ������
DWORD dwOneLevelScores;
GAME_COORD ball;
// ����Ʒ֣���ʼΪ0
int score = 0;
// ���漶�𣬳�ʼΪ0
int level = 0;

/************** �������� ****************/
// ���ػ���
int GetScore() { return score; }

// ����level
int GetLevel() { return level; }

// ����ٶȡ�
BOOL SpeedUp(HWND hwnd)
{
	// ������ʱ����ʱʱ��
	dwTimerElapse = (DWORD)(dwTimerElapse * dbLevelSpeedupRatio);
	KillTimer(hwnd, TIMER_ID);
	SetTimer(hwnd, TIMER_ID, dwTimerElapse, NULL);
	return TRUE;
}
// ��Ϸ���Ƶ�һ����Ҫ���̡�
// ����ʱ������ʱ���еĴ����߼���
void OnTimer(HWND hwnd)
{
	//��ʱ����ʱ�Ժ�С������һ��
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
		//�ж��Ƿ񽵼�,�����µĵȼ�
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
// ��Ϸ�ĳ�ʼ����
// ������Ϸ���ڲ����ݽṹ��ϵͳ����
void CreateGame(HWND hwnd, // �����ھ��
	DWORD dwInitTimerElapse, //
	unsigned int one_level_scores,
	DOUBLE level_speedup_ratio,
	int boundary_x, int boundary_y,
	int init_x, int init_y,
	int lenth)
{
	// �������������
	// ��Ҫʹ�����������С���λ�õȡ�
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	srand(ft.dwLowDateTime);

	dbLevelSpeedupRatio = level_speedup_ratio;
	dwTimerElapse = dwInitTimerElapse;
	dwOneLevelScores = one_level_scores;

	// ������Ϸ�ı߽�
	SetBoundary(boundary_x, boundary_y);
	// ������ʾplate�����ݽṹ
	CreatePlate(init_x,init_y, lenth);
	// ������ʾball�����ݽṹ
	CreateBall();
	SetTimer(hwnd, TIMER_ID, dwTimerElapse, NULL);
}