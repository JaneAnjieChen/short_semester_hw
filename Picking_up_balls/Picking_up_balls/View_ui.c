#include<windows.h>
#include"Plate.h"
//像素点个数
#define CELL_PIXEL 20
//#define CELL_PIXEL2 30
// 用来绘图的颜色
#define COLOR_PLATE RGB(255,0,0)
#define COLOR_BALL RGB(0,255,0)
#define COLOR_BOUNDARY RGB(0,255,255)
#define COLOR_TEXT RGB(0,0,255)

//游戏参数设置
#define INIT_TIMER_ELAPSE 300
#define ONE_LEVEL_SCORES 5
#define PLATE_LENTH 3
#define SPEEDUP_RATIO 0.9
#define MAX_X 30
#define MAX_Y 30
#define INIT_X 10//盘子横坐标的初始位置
#define Y 25//盘子的纵坐标

//全局标量
HINSTANCE hinst;
RECT rectBoundary;

// 函数声明
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL SpeedUp(HWND hwnd);

//入口函数WinMain
int WINAPI WinMain(
	HINSTANCE hinstance, // 程序实例句柄，在程序运行，进程创建后，由操作系统向应用程序传入
	HINSTANCE hPrevInstance, // 父进程的程序实例句柄
	LPSTR lpCmdLine,  // 命令行参数，地位和作用类似C语言main函数参数argc和argv,但是没有按空格进行切分
	int nCmdShow)   // 用于指明窗口是否需要显示的参数。
{
	WNDCLASS wc;
	HWND hwnd;
	MSG msg;
	int fGotMessage;
	hinst = hinstance;
	// 窗口类的样式，这里设置的样式表示窗口在大小变化是需要重绘
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// 一个函数指针，这个函数用来处理窗口消息。 详见 MainWndProc函数的注释。
	wc.lpfnWndProc = MainWndProc;//!
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hinstance;
	wc.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL,IDC_CROSS);
	wc.hbrBackground = (HBRUSH)GetStockObject(
		BLACK_BRUSH);
	wc.lpszMenuName = "MainMenu";
	wc.lpszClassName = "MainWClass";


	if (!RegisterClass(&wc))
	{
		// 窗口注册失败，消息框提示，并退出。
		MessageBox(NULL, "创建窗口class失败", "错误！", MB_ICONERROR | MB_OK);
		return -1;
	}

// 窗口注册成功，继续运行。

// Create the main window. 
hwnd = CreateWindow(
	"MainWClass",			// 窗口类名，必须是已经注册了的窗口类
	"Picking up balls!",		// title-bar string 
	WS_OVERLAPPEDWINDOW,	// 窗口的style，这个表示为top-level window 
	CW_USEDEFAULT,			// 窗口水平位置default horizontal POINT 
	CW_USEDEFAULT,	// 窗口垂直位置default vertical POINT 
	CW_USEDEFAULT,			// 窗口宽度 default width 
	CW_USEDEFAULT,			// 窗口高度 default height 
	(HWND)NULL,				// 父窗口句柄 no owner window 句柄handle操作一个事物的接口**
	(HMENU)NULL,			// 窗口菜单的句柄 use class menu 
	hinst,				// 应用程序实例句柄 handle to application instance 
	(LPVOID)NULL);	// 指向附加数据的指针 no window-creation data 

	if (!hwnd)
	{
		// 窗口创建失败，消息框提示，并退出。
		MessageBox(NULL, "创建窗口失败", "错误！", MB_ICONERROR | MB_OK);
		return -1;
	}
	//窗口创建成功，继续运行。

	ShowWindow(hwnd, nCmdShow);//showWindow
	UpdateWindow(hwnd);


	while ((fGotMessage = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0
		&& fGotMessage != -1)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (msg.wParam);
}

void GamePaint(HWND hwnd)
{
	HPEN hpen;
	HDC hdc, hdcmem;
	HBITMAP hbmMem;

	HPEN hPenBoundary;
	HPEN hOldPen;

	HBRUSH hbrushBall;
	HBRUSH hBrushPlate;
	HBRUSH hOldBrush;

	HFONT hFont, hOldFont;

	RECT rect;
	
	PGAME_COORD pPlateBody;
	PGAME_COORD lpBall;
	int i, plate_size;

	GetClientRect(hwnd, &rect);
	hdc = GetDC(hwnd);
	hdcmem = CreateCompatibleDC(hdc);//拼接好再放入物理DC中，为了速度，为了避免频繁画图频闪
	hbmMem = CreateCompatibleBitmap(hdc,
		rect.right - rect.left, rect.bottom - rect.top);//创建物理位图

	SelectObject(hdcmem, hbmMem);

	// 创建需要用到的PEN和BRUSH
	hbrushBall = CreateSolidBrush(COLOR_BALL); 
	hpen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
	hBrushPlate = CreateSolidBrush(COLOR_PLATE);
	hPenBoundary = CreatePen(0, 5, COLOR_BOUNDARY);

	/*******************************************************************************
	* #############  画背景  ################
	*
	*******************************************************************************/
	FillRect(hdcmem, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));

	/*******************************************************************************
	* #############  画小球 ################
	*
	*******************************************************************************/
	// 将画图需要用的PEN和BRUSH选择到DC中
	hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrushBall);
	hOldPen = (HPEN)SelectObject(hdcmem, hpen);

	lpBall = GetBall();
	Ellipse(hdcmem,
		lpBall->x*CELL_PIXEL+rectBoundary.left,
		lpBall->y * CELL_PIXEL + rectBoundary.top,
		(lpBall->x + 1)*CELL_PIXEL + rectBoundary.left,
		(lpBall->y + 1)*CELL_PIXEL + rectBoundary.top);

	/*******************************************************************************
	* #############  画盘子 ################
	*
	*******************************************************************************/
	SelectObject(hdcmem, hBrushPlate);

	plate_size =3;

	for (i = 0; i <  plate_size; i++)
	{
		pPlateBody = (PGAME_COORD)GetPlateAt(i);
		Rectangle(hdcmem,
			pPlateBody->x *CELL_PIXEL + rectBoundary.left,
			pPlateBody->y *CELL_PIXEL + rectBoundary.top,
			(pPlateBody->x + 1)*CELL_PIXEL+ rectBoundary.left,
			(pPlateBody->y + 1)*CELL_PIXEL+ rectBoundary.top);
	}

	/*******************************************************************************
	* #############  画墙  ################
	*
	*******************************************************************************/
	SelectObject(hdcmem, hPenBoundary);
	// 将PEN移动到需要绘制的方框的左下角
	MoveToEx(hdcmem, rectBoundary.left, rectBoundary.bottom, NULL);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.bottom);

	/*******************************************************************************
	* #############  写一行字  ################
	*
	********************************************************************************/
	// 创建了一个字体对象
	hFont = CreateFont(48, 0, 0, 0, FW_DONTCARE, 0, 1, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Consolas"));

	// 将这个FONT对象放入DC中
	if (hOldFont = (HFONT)SelectObject(hdcmem, hFont))
	{
		CHAR szSourceInfo[1024];//szSourseInfo Type: LPTSTR，The buffer that is to receive the formatted output.The maximum size of the buffer is 1, 024 bytes.
		wsprintf(szSourceInfo, "score %d level %d", GetScore(), GetLevel());
		// 设置输出颜色 
		SetTextColor(hdcmem, COLOR_TEXT);
		// 输出字符串。
		TextOut(hdcmem, rectBoundary.left + 3, rectBoundary.bottom + 3,
			szSourceInfo, lstrlen(szSourceInfo));
		// 输出完成，将原来的字体对象放回DC中
		SelectObject(hdcmem, hOldFont);
	}

	// 在内存DC中画完，一次输出的窗口DC上。
	BitBlt(hdc,
		0, 0, rect.right - rect.left, rect.bottom - rect.top,
		hdcmem, 0, 0, SRCCOPY);//内存拷贝，还可以进行缩放和局部拷贝

	/*******************************************************************************
	* #############  回收和释放资源  ################
	*
	*******************************************************************************/
	// 回收资源
	// DeleteObject 释放GDI对象
	DeleteObject(hbmMem);
	DeleteObject(hdcmem);
	DeleteObject(hbrushBall);
	DeleteObject(hBrushPlate);
	DeleteObject(hpen);
	DeleteObject(hPenBoundary);
	DeleteObject(hFont);
	/*******************************************************************************
	* #############  ReleaseDC 函数  ################
	* 释放占用的DC等系统资源。
	*******************************************************************************/
	ReleaseDC(hwnd, hdc);
}


/*******************************************************************************
* ##########   根据游戏界面的坐标系大小设置游戏窗口大小    ##########
*******************************************************************************/

void ReSizeGameWnd(HWND hwnd)
{
	POINT ptLeftTop;		// 左上角
	POINT ptRightBottom;	// 右下角
	RECT rectWindow;
	PGAME_COORD pCoordBoundary = GetBoundary();

	// 设置游戏边界
	rectBoundary.left = 10;
	rectBoundary.top = 10;
	rectBoundary.right = 10 + CELL_PIXEL * (pCoordBoundary->x + 1);
	rectBoundary.bottom = 10 + CELL_PIXEL * (pCoordBoundary->y + 1);

	// 计算上下左右角的位置
	ptLeftTop.x = rectBoundary.left;
	ptLeftTop.y = rectBoundary.top;
	ptRightBottom.x = rectBoundary.right;
	ptRightBottom.y = rectBoundary.bottom;
	ClientToScreen(hwnd, &ptLeftTop);
	ClientToScreen(hwnd, &ptRightBottom);

	GetWindowRect(hwnd, &rectWindow);
	// 计算好了，设置窗口大小。
	MoveWindow(hwnd,
		rectWindow.left,
		rectWindow.top,
		ptLeftTop.x - rectWindow.left + ptRightBottom.x - rectWindow.left, // 保存边界和左右两边边框相等。
		rectBoundary.bottom + 120, //给积分信息留出显示空间。
		TRUE);
}

LONG CALLBACK MainWndProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_CREATE:
			CreateGame(hwnd,
				INIT_TIMER_ELAPSE,
				ONE_LEVEL_SCORES,
				SPEEDUP_RATIO,
				MAX_X, MAX_Y,
				INIT_X, Y,
				PLATE_LENTH);
			ReSizeGameWnd(hwnd);
			//SetTimer(hWnd, 12039, 100, NULL);
			break;
		case WM_PAINT:

			GamePaint(hwnd);
			break;

		case WM_KEYDOWN:

			OnKeyDown(wParam,hwnd);
			GamePaint(hwnd);
			break;

		case WM_LBUTTONDOWN:
			OnTimer(hwnd);
			GamePaint(hwnd);
			break;

		case WM_TIMER:

			OnTimer(hwnd);
			GamePaint(hwnd);
			break;

		case WM_DESTROY:
			ExitProcess(0);
			break;

		default:
			break;
}
	//GamePaint(hwnd);
    return DefWindowProc(hwnd, msg, wParam, lParam);
	


}