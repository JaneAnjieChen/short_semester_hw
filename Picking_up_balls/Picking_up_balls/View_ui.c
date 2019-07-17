#include<windows.h>
#include"Plate.h"
//���ص����
#define CELL_PIXEL 20
//#define CELL_PIXEL2 30
// ������ͼ����ɫ
#define COLOR_PLATE RGB(255,0,0)
#define COLOR_BALL RGB(0,255,0)
#define COLOR_BOUNDARY RGB(0,255,255)
#define COLOR_TEXT RGB(0,0,255)

//��Ϸ��������
#define INIT_TIMER_ELAPSE 300
#define ONE_LEVEL_SCORES 5
#define PLATE_LENTH 3
#define SPEEDUP_RATIO 0.9
#define MAX_X 30
#define MAX_Y 30
#define INIT_X 10//���Ӻ�����ĳ�ʼλ��
#define Y 25//���ӵ�������

//ȫ�ֱ���
HINSTANCE hinst;
RECT rectBoundary;

// ��������
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL SpeedUp(HWND hwnd);

//��ں���WinMain
int WINAPI WinMain(
	HINSTANCE hinstance, // ����ʵ��������ڳ������У����̴������ɲ���ϵͳ��Ӧ�ó�����
	HINSTANCE hPrevInstance, // �����̵ĳ���ʵ�����
	LPSTR lpCmdLine,  // �����в�������λ����������C����main��������argc��argv,����û�а��ո�����з�
	int nCmdShow)   // ����ָ�������Ƿ���Ҫ��ʾ�Ĳ�����
{
	WNDCLASS wc;
	HWND hwnd;
	MSG msg;
	int fGotMessage;
	hinst = hinstance;
	// ���������ʽ���������õ���ʽ��ʾ�����ڴ�С�仯����Ҫ�ػ�
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// һ������ָ�룬�������������������Ϣ�� ��� MainWndProc������ע�͡�
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
		// ����ע��ʧ�ܣ���Ϣ����ʾ�����˳���
		MessageBox(NULL, "��������classʧ��", "����", MB_ICONERROR | MB_OK);
		return -1;
	}

// ����ע��ɹ����������С�

// Create the main window. 
hwnd = CreateWindow(
	"MainWClass",			// �����������������Ѿ�ע���˵Ĵ�����
	"Picking up balls!",		// title-bar string 
	WS_OVERLAPPEDWINDOW,	// ���ڵ�style�������ʾΪtop-level window 
	CW_USEDEFAULT,			// ����ˮƽλ��default horizontal POINT 
	CW_USEDEFAULT,	// ���ڴ�ֱλ��default vertical POINT 
	CW_USEDEFAULT,			// ���ڿ�� default width 
	CW_USEDEFAULT,			// ���ڸ߶� default height 
	(HWND)NULL,				// �����ھ�� no owner window ���handle����һ������Ľӿ�**
	(HMENU)NULL,			// ���ڲ˵��ľ�� use class menu 
	hinst,				// Ӧ�ó���ʵ����� handle to application instance 
	(LPVOID)NULL);	// ָ�򸽼����ݵ�ָ�� no window-creation data 

	if (!hwnd)
	{
		// ���ڴ���ʧ�ܣ���Ϣ����ʾ�����˳���
		MessageBox(NULL, "��������ʧ��", "����", MB_ICONERROR | MB_OK);
		return -1;
	}
	//���ڴ����ɹ����������С�

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
	hdcmem = CreateCompatibleDC(hdc);//ƴ�Ӻ��ٷ�������DC�У�Ϊ���ٶȣ�Ϊ�˱���Ƶ����ͼƵ��
	hbmMem = CreateCompatibleBitmap(hdc,
		rect.right - rect.left, rect.bottom - rect.top);//��������λͼ

	SelectObject(hdcmem, hbmMem);

	// ������Ҫ�õ���PEN��BRUSH
	hbrushBall = CreateSolidBrush(COLOR_BALL); 
	hpen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
	hBrushPlate = CreateSolidBrush(COLOR_PLATE);
	hPenBoundary = CreatePen(0, 5, COLOR_BOUNDARY);

	/*******************************************************************************
	* #############  ������  ################
	*
	*******************************************************************************/
	FillRect(hdcmem, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));

	/*******************************************************************************
	* #############  ��С�� ################
	*
	*******************************************************************************/
	// ����ͼ��Ҫ�õ�PEN��BRUSHѡ��DC��
	hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrushBall);
	hOldPen = (HPEN)SelectObject(hdcmem, hpen);

	lpBall = GetBall();
	Ellipse(hdcmem,
		lpBall->x*CELL_PIXEL+rectBoundary.left,
		lpBall->y * CELL_PIXEL + rectBoundary.top,
		(lpBall->x + 1)*CELL_PIXEL + rectBoundary.left,
		(lpBall->y + 1)*CELL_PIXEL + rectBoundary.top);

	/*******************************************************************************
	* #############  ������ ################
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
	* #############  ��ǽ  ################
	*
	*******************************************************************************/
	SelectObject(hdcmem, hPenBoundary);
	// ��PEN�ƶ�����Ҫ���Ƶķ�������½�
	MoveToEx(hdcmem, rectBoundary.left, rectBoundary.bottom, NULL);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.bottom);

	/*******************************************************************************
	* #############  дһ����  ################
	*
	********************************************************************************/
	// ������һ���������
	hFont = CreateFont(48, 0, 0, 0, FW_DONTCARE, 0, 1, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Consolas"));

	// �����FONT�������DC��
	if (hOldFont = (HFONT)SelectObject(hdcmem, hFont))
	{
		CHAR szSourceInfo[1024];//szSourseInfo Type: LPTSTR��The buffer that is to receive the formatted output.The maximum size of the buffer is 1, 024 bytes.
		wsprintf(szSourceInfo, "score %d level %d", GetScore(), GetLevel());
		// ���������ɫ 
		SetTextColor(hdcmem, COLOR_TEXT);
		// ����ַ�����
		TextOut(hdcmem, rectBoundary.left + 3, rectBoundary.bottom + 3,
			szSourceInfo, lstrlen(szSourceInfo));
		// �����ɣ���ԭ�����������Ż�DC��
		SelectObject(hdcmem, hOldFont);
	}

	// ���ڴ�DC�л��꣬һ������Ĵ���DC�ϡ�
	BitBlt(hdc,
		0, 0, rect.right - rect.left, rect.bottom - rect.top,
		hdcmem, 0, 0, SRCCOPY);//�ڴ濽���������Խ������ź;ֲ�����

	/*******************************************************************************
	* #############  ���պ��ͷ���Դ  ################
	*
	*******************************************************************************/
	// ������Դ
	// DeleteObject �ͷ�GDI����
	DeleteObject(hbmMem);
	DeleteObject(hdcmem);
	DeleteObject(hbrushBall);
	DeleteObject(hBrushPlate);
	DeleteObject(hpen);
	DeleteObject(hPenBoundary);
	DeleteObject(hFont);
	/*******************************************************************************
	* #############  ReleaseDC ����  ################
	* �ͷ�ռ�õ�DC��ϵͳ��Դ��
	*******************************************************************************/
	ReleaseDC(hwnd, hdc);
}


/*******************************************************************************
* ##########   ������Ϸ���������ϵ��С������Ϸ���ڴ�С    ##########
*******************************************************************************/

void ReSizeGameWnd(HWND hwnd)
{
	POINT ptLeftTop;		// ���Ͻ�
	POINT ptRightBottom;	// ���½�
	RECT rectWindow;
	PGAME_COORD pCoordBoundary = GetBoundary();

	// ������Ϸ�߽�
	rectBoundary.left = 10;
	rectBoundary.top = 10;
	rectBoundary.right = 10 + CELL_PIXEL * (pCoordBoundary->x + 1);
	rectBoundary.bottom = 10 + CELL_PIXEL * (pCoordBoundary->y + 1);

	// �����������ҽǵ�λ��
	ptLeftTop.x = rectBoundary.left;
	ptLeftTop.y = rectBoundary.top;
	ptRightBottom.x = rectBoundary.right;
	ptRightBottom.y = rectBoundary.bottom;
	ClientToScreen(hwnd, &ptLeftTop);
	ClientToScreen(hwnd, &ptRightBottom);

	GetWindowRect(hwnd, &rectWindow);
	// ������ˣ����ô��ڴ�С��
	MoveWindow(hwnd,
		rectWindow.left,
		rectWindow.top,
		ptLeftTop.x - rectWindow.left + ptRightBottom.x - rectWindow.left, // ����߽���������߱߿���ȡ�
		rectBoundary.bottom + 120, //��������Ϣ������ʾ�ռ䡣
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