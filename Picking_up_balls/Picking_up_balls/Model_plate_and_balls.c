/********************************************************************************
* ##########  谈谈封装 ##########
* “封装”是程序不会乱掉的一个重要技能。
* 我们要写出结构化、易于阅读、理解和维护的代码。采用封装是比较容易掌握的方法。
*
* 具体而言：
*
* 1、先考虑好这个程序的逻辑和状态需要多少数据来表示，
* 比如这个接小球游戏，我们使用这些数据就可以表示全部游戏状态。
* // 小球
* GAME_COORD ball;
* // 盘子（链表）
* PLIST plate_list;
* // 蛇移动方向
* dirction plate_dir;
* // 游戏边界（这是右下，左上为0,0）
* GAME_COORD boundary;
*
* 2、写出一系列接口函数来操作这些数据。所谓接口函数，满足一下几个特征：
* (1) 这些数据除了接口函数以外，其他代码都不会直接访问。
* (2) 所有其他代码都只能通过调用这些接口函数来改变程序的状态，而不要直接去访问数据。
* 如本文件的若干函数都是对上述数据的操作。

* 这样实现了：对于上层代码，并不需要知道数据的操作逻辑，也不需要知道数据结构，而只需要调用函数即可。
* 对上次代码而言，逻辑简单了很多。
* 同样，如果数据结构和内部逻辑有了修改，那么可以做到接口函数的原型不变，
* 上层代码不变，而只需要修改接口函数的内部实现过程即可。
*
* 这就称之为“封装”，或者称之为 “数据的封装和接口化”
*
*******************************************************************************/

/************** 头文件 ****************/
#include"Plate.h"
/************* 全局变量 ****************/
//小球
GAME_COORD ball;

//球（链表）
PLIST plate_list;

//盘子移动方向
direction plate_dir;

// 游戏边界（这是右下，左上为0,0）
GAME_COORD boundary;

/*********** 内部函数申明 **************/
/// 判断两个坐标是否相等。
int CoordEqual(PGAME_COORD one, PGAME_COORD two);
///判断分数是否低于0
int IsScoreBelow0();
PGAME_COORD GetPlateTail();

/**************函数定义****************/
// 判断两个坐标 GAME_COORD 是否重合
int CoordEqual(PGAME_COORD one, PGAME_COORD two)
{
	if (one->x == two->x && one->y == two->y)
		return 1;
	else
		return 0;
}

//设置边界坐标
void SetBoundary(int x, int y)
{
	boundary.x = x;
	boundary.y = y;
}

// 获得边界坐标
PGAME_COORD GetBoundary()
{
	return &boundary;
}

//生成新的小球
int CreateBall()
{
	PGAME_COORD posbody;
    //随机生成小球横坐标
	ball.x = rand() % boundary.x;
	ball.y = 0;
	return 1;
}



//获得小球的坐标
PGAME_COORD GetBall()
{
	return &ball;
}

//创建盘子
void CreatePlate( int head_x,
	int head_y, int lenth)
{
	PGAME_COORD p;
	PGAME_COORD pNew;
	PGAME_COORD pTail;		// 倒数第一节点
	//PGAME_COORD pLastButOne;	// 倒数第二节点
	int i;
	lenth = 3;
	plate_list = ListCreate(NULL);
	p = (PGAME_COORD)malloc(sizeof(GAME_COORD));
	// 盘子头部的初始位置
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
//判断分数是否小于0
int IsScoreBelow0()
{
	if (GetScore() < 0)
		return PLATE_BROKEN;
	else
		return PLATE_MOVED;
}

// 销毁盘子，释放内存资源
void DistroyPlate()
{
	ListDistoryAndFree(plate_list);
	return;
}

// 销毁小球，释放内存资源
void DistroyBall()
{
	//ListDistoryAndFree(ball);
	free(&ball);
	return;
}

//用来移动一步，
//移动以后，如果接到小球，则分数加1
int PlateMove()
{
	int i;
	// 头和尾的坐标
	PGAME_COORD posHead;
	PGAME_COORD posBody;
	PGAME_COORD posTail;
	// 把尾从链表中取出，按照当前方向放置到头的位置。
	posHead = (PGAME_COORD)ListGetAt(plate_list, 0);// 获得0位置上节点中的数据。
	posTail = (PGAME_COORD)ListGetAt(plate_list, 2);
	/*posTail = (PGAME_COORD)ListPopBack(plate_list);*/
	// 根据当前方向来设定新的头坐标。
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

	//判断是否接到了小球
    //从第一个结点开始逐一和小球比较
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
// 按照序号获得盘子的节点的坐标，不能超过盘子的长度，否则返回NULL
PGAME_COORD GetPlateAt(int n)
{
	if (n < ListSize(plate_list))
		return (PGAME_COORD)ListGetAt(plate_list, n);
	else
		return NULL;
}

//改变盘子移动的方向
void SetDirection(direction dir)
{
	plate_dir = dir;
}

// 获得当前盘子的方向
direction GetDirection()
{
	return plate_dir;
}

