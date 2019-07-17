#include<Windows.h>
#include"LinkList'.h"
//�������ӵ�״̬
#define PLATE_MOVED 0
#define PLATE_BROKEN 1
#define PLATE_GOTTEN_BALL 2
#define PLATE_MISSED_BALL 3
#define PLATE_ERROR 4

//���ӵķ���,ö������
typedef enum _direction
{
	PLATE_RIGHT,
	PLATE_LEFT,
}direction;

//���꣬coordinate
typedef struct _GAME_COORD
{
	short x;
	short y;
}GAME_COORD, *PGAME_COORD;

//��������

///Model_plate_and_balls.c�еĽӿں���

//���ñ߽�����
void SetBoundary(int x, int y);

//��ñ߽�����
PGAME_COORD GetBoundary();

//�����µ�С��
int CreateBall();

//���С������
PGAME_COORD GetBall();

//��������
void CreatePlate(int head_x, int head_y,int lenth);

//�������ӣ��ͷ��ڴ�
void DistroyPlate();

//����С���ͷ��ڴ�
void DistoryBall();

//�����ƶ�һ��������ӵ�С�򣬷�����1
int PlateMove();

//������ӵ�һ���ڵ������
PGAME_COORD GetPlateHead();

//����������һ���ڵ������
PGAME_COORD GetSnakeTail();

//������Ż�����ӵĽ������꣬���ܳ������ӳ��ȣ����򷵻�NULL
PGAME_COORD GetPlateAt(int n);

//�ı������ƶ��ķ���
void SetDirection(direction dir);

//��õ�ǰ���ӵķ���
direction GetDirection();

///Controller_game_operation.c�еĽӿں���

int GetScore();
int GetLevel();
void CreateGame(HWND hwnd,DWORD dwInitTimerElapse,
	unsigned int one_level_scores,DOUBLE level_speedup_radio,
	int boundary_x,int boundary_y,
	int init_x,int init_y,int lenth);
void OnTimer(HWND hwnd);
void OnKeyDown(DWORD vk,HWND hwnd);

void SetDirection(direction dir);


