/********************************************************************************
* ##########  ̸̸��װ ##########
* ����װ���ǳ��򲻻��ҵ���һ����Ҫ���ܡ�
* ����Ҫд���ṹ���������Ķ�������ά���Ĵ��롣���÷�װ�ǱȽ��������յķ�����
*
* ������ԣ�
*
* 1���ȿ��Ǻ����������߼���״̬��Ҫ������������ʾ��
* ���������С����Ϸ������ʹ����Щ���ݾͿ��Ա�ʾȫ����Ϸ״̬��
* // С��
* GAME_COORD ball;
* // ���ӣ�����
* PLIST plate_list;
* // ���ƶ�����
* dirction plate_dir;
* // ��Ϸ�߽磨�������£�����Ϊ0,0��
* GAME_COORD boundary;
*
* 2��д��һϵ�нӿں�����������Щ���ݡ���ν�ӿں���������һ�¼���������
* (1) ��Щ���ݳ��˽ӿں������⣬�������붼����ֱ�ӷ��ʡ�
* (2) �����������붼ֻ��ͨ��������Щ�ӿں������ı�����״̬������Ҫֱ��ȥ�������ݡ�
* �籾�ļ������ɺ������Ƕ��������ݵĲ�����

* ����ʵ���ˣ������ϲ���룬������Ҫ֪�����ݵĲ����߼���Ҳ����Ҫ֪�����ݽṹ����ֻ��Ҫ���ú������ɡ�
* ���ϴδ�����ԣ��߼����˺ܶࡣ
* ͬ����������ݽṹ���ڲ��߼������޸ģ���ô���������ӿں�����ԭ�Ͳ��䣬
* �ϲ���벻�䣬��ֻ��Ҫ�޸Ľӿں������ڲ�ʵ�ֹ��̼��ɡ�
*
* ��ͳ�֮Ϊ����װ�������߳�֮Ϊ �����ݵķ�װ�ͽӿڻ���
*
*******************************************************************************/

/************** ͷ�ļ� ****************/
#include"Plate.h"
/************* ȫ�ֱ��� ****************/
//С��
GAME_COORD ball;

//������
PLIST plate_list;

//�����ƶ�����
direction plate_dir;

// ��Ϸ�߽磨�������£�����Ϊ0,0��
GAME_COORD boundary;

/*********** �ڲ��������� **************/
/// �ж����������Ƿ���ȡ�
int CoordEqual(PGAME_COORD one, PGAME_COORD two);
///�жϷ����Ƿ����0
int IsScoreBelow0();
PGAME_COORD GetPlateTail();

/**************��������****************/
// �ж��������� GAME_COORD �Ƿ��غ�
int CoordEqual(PGAME_COORD one, PGAME_COORD two)
{
	if (one->x == two->x && one->y == two->y)
		return 1;
	else
		return 0;
}

//���ñ߽�����
void SetBoundary(int x, int y)
{
	boundary.x = x;
	boundary.y = y;
}

// ��ñ߽�����
PGAME_COORD GetBoundary()
{
	return &boundary;
}

//�����µ�С��
int CreateBall()
{
	PGAME_COORD posbody;
    //�������С�������
	ball.x = rand() % boundary.x;
	ball.y = 0;
	return 1;
}



//���С�������
PGAME_COORD GetBall()
{
	return &ball;
}

//��������
void CreatePlate( int head_x,
	int head_y, int lenth)
{
	PGAME_COORD p;
	PGAME_COORD pNew;
	PGAME_COORD pTail;		// ������һ�ڵ�
	//PGAME_COORD pLastButOne;	// �����ڶ��ڵ�
	int i;
	lenth = 3;
	plate_list = ListCreate(NULL);
	p = (PGAME_COORD)malloc(sizeof(GAME_COORD));
	// ����ͷ���ĳ�ʼλ��
	p->x = head_x;
	p->y = head_y;
	ListPushFront(plate_list, p);
	for (i = 1; i <lenth; i++)//0,1,2,3
	{
		pNew = (PGAME_COORD)malloc(sizeof(GAME_COORD));
		pNew->x = head_x + 1;
		pNew->y = head_y;
		ListPushFront(plate_list, pNew);
	}
	return;
}
//�жϷ����Ƿ�С��0
int IsScoreBelow0()
{
	if (GetScore() < 0)
		return PLATE_BROKEN;
	else
		return PLATE_MOVED;
}

// �������ӣ��ͷ��ڴ���Դ
void DistroyPlate()
{
	ListDistoryAndFree(plate_list);
	return;
}

// ����С���ͷ��ڴ���Դ
void DistroyBall()
{
	//ListDistoryAndFree(ball);
	free(&ball);
	return;
}

//�����ƶ�һ����
//�ƶ��Ժ�����ӵ�С���������1
int PlateMove()
{
	int i;
	// ͷ��β������
	PGAME_COORD posHead;
	PGAME_COORD posBody;
	PGAME_COORD posTail;
	// ��β��������ȡ�������յ�ǰ������õ�ͷ��λ�á�
	posHead = (PGAME_COORD)ListGetAt(plate_list, 0);// ���0λ���Ͻڵ��е����ݡ�
	posTail = (PGAME_COORD)ListGetAt(plate_list, 2);
	/*posTail = (PGAME_COORD)ListPopBack(plate_list);*/
	// ���ݵ�ǰ�������趨�µ�ͷ���ꡣ
	direction plate_dir_pre = GetDirection();
	if (plate_dir_pre == PLATE_LEFT && plate_dir == PLATE_LEFT)
	{
		posTail->x = posHead->x - 1;
		posTail->y = posHead->y;
	}
	else if (plate_dir_pre == PLATE_RIGHT && plate_dir == PLATE_RIGHT)
	{
		posTail->x = posHead->x + 1;
		posTail->y = posHead->y;
	}
	else if(plate_dir_pre==PLATE_RIGHT && plate_dir==PLATE_LEFT)
	{
		posHead->x = posTail->x ;
		posTail->y = posHead->y;
	}
	else if (plate_dir_pre == PLATE_LEFT && plate_dir_pre == PLATE_RIGHT)
	{
		posHead->x = posTail->x ;
		posTail->y = posHead->y;
	}
	/*switch (plate_dir)
	{
	case PLATE_LEFT:
		posTail->x = posHead->x-1 ;
		posTail->y = posHead->y;
		break;
	case PLATE_RIGHT:
		posTail->x = posHead->x + 1;
		posTail->y = posHead->y;
		break;
	}*/
	ListPushFront(plate_list, posTail);

	//�ж��Ƿ�ӵ���С��
    //�ӵ�һ����㿪ʼ��һ��С��Ƚ�
	int lenth=3;
	for (i = 0; i < lenth; i++)
	{
		posBody = (PGAME_COORD)ListGetAt(plate_list, i);
		if (CoordEqual(posBody, &ball))
		{
			//DistoryBall();
			//free(ball.x);
			//free(ball.y);
			return PLATE_GOTTEN_BALL;
		}
		if (ball.y > posHead->y)
		{
			return PLATE_MISSED_BALL;
		}
	}
	return IsScoreBelow0();
}
int GetPlateSize()
{
	return ListSize(plate_list);
}
PGAME_COORD GetPlateHead()
{
	return(PGAME_COORD)ListGetAt(plate_list, 0);
}
PGAME_COORD GetPlateTail()
{
	return (PGAME_COORD)ListGetAt(plate_list, 2);
}
// ������Ż�����ӵĽڵ�����꣬���ܳ������ӵĳ��ȣ����򷵻�NULL
PGAME_COORD GetPlateAt(int n)
{
	if (n < ListSize(plate_list))
		return (PGAME_COORD)ListGetAt(plate_list, n);
	else
		return NULL;
}

//�ı������ƶ��ķ���
void SetDirection(direction dir)
{
	plate_dir = dir;
}

// ��õ�ǰ���ӵķ���
direction GetDirection()
{
	return plate_dir;
}

