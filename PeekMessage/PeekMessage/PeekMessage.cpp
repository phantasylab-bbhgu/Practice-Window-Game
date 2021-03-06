// PeekMessage.cpp: 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "PeekMessage.h"
#include<list>
#include<math.h>

using namespace std;

#define MAX_LOADSTRING 100

typedef struct _tagRectangle
{
	float left;
	float top;
	float right;
	float bottom;
}RECTANGLE;

typedef struct _tagSphere
{
	float x;
	float y;
	float radius;
}SPHERE, *PSPHERE;

typedef struct _tagBullet
{
	SPHERE tSphere;
	float fDist;
	float fLimitDist;

}BULLET, *PBULLET;

typedef struct _tagMonster
{
	SPHERE tSphere;
	float fSpeed;
	float fTime;
	float fLimitTime;
	int iDir;
}MONSTER, *PMONSTER;

enum MOVE_DIR
{
	MD_BACK = -1,
	MD_IDLE,
	MD_FRONT
};

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HDC g_hDC;
HWND g_hWnd;
bool g_bLoop = true;
RECTANGLE g_tPlayerRC = { 100, 100, 200, 200 };
MONSTER g_tMonster;


// 플레이어 총알
list<BULLET> g_PlayerBulletList;

// 몬스터 총알
list<BULLET> g_MonsterBulletList;


// 시간을 구하기 위한 변수들
LARGE_INTEGER g_tSecond;
LARGE_INTEGER g_tTime;
float g_fDeltaTime;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void Run();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PEEKMESSAGE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	//화면용 DC 생성
	g_hDC = GetDC(g_hWnd);

	// 몬스터 초기화
	g_tMonster.tSphere.x = 800.0f - 50.0f;
	g_tMonster.tSphere.y = 50.0f;
	g_tMonster.tSphere.radius = 50.0f;
	g_tMonster.fSpeed = 300.0f;
	g_tMonster.fTime = 0.0f;
	g_tMonster.fLimitTime = 1.0f;
	g_tMonster.iDir = MD_FRONT;


    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PEEKMESSAGE));

    MSG msg;

	QueryPerformanceFrequency(&g_tSecond);
	QueryPerformanceCounter(&g_tTime);

    // 기본 메시지 루프입니다.
    while (g_bLoop)
    {
		//PeekMessage는 메시지가 메시지큐에 없어도 바로 빠져나온다.
		//메시지가 있을 경우 true, 없을 경우 false
		//메시지가 없는 시간이 윈도의 데드타임
		//PM_REMOVE는 메시지가 있을 경우 가져오면서, 그 메시지를 지워버림
        if (PeekMessage(&msg, nullptr, 0, 0,PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		//윈도우 데드타임인 경우
		else
		{
			//이 부분에서 게임을 구현함
			//윈도우의 이벤트가 발생하지 않는 타임

			Run();
		}
    }

	ReleaseDC(g_hWnd, g_hDC);

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PEEKMESSAGE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_PEEKMESSAGE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;

   // 실제 윈도우 타이틀바나 메뉴를 포함한 윈도의 크기를 구해준다.
   
   RECT rc = { 0,0,800,600 };
   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

   // 위에서 구해준 크기로 윈도우 클라이언트 영역의 크기를
   // 원하는 크기로 맞춰줘야 한다. (윈도우 창의 위치 등등..)
   SetWindowPos(hWnd, HWND_TOPMOST, 100, 100, rc.right - rc.left, 
	   rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
            EndPaint(hWnd, &ps);
        }
        break;
		
		//윈도우 종료될 때 들어오는 메시지
    case WM_DESTROY:
		g_bLoop = false;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void Run()
{
	//이벤트가 없을때만 들어옴

	// DeltaTime 구하기
	LARGE_INTEGER tTime;
	QueryPerformanceCounter(&tTime);
	g_fDeltaTime = (tTime.QuadPart - g_tTime.QuadPart) / (float)g_tSecond.QuadPart;
	g_tTime = tTime;
	//

	//Time Scale
	static float fTimeScale = 1.0f;
	if (GetAsyncKeyState(VK_F1))
	{
		fTimeScale -= g_fDeltaTime;

		if (fTimeScale < 0.f)
		{
			fTimeScale = 0.f;
		}
	}
	//

	// 플레이어 초당 이동속도 : 300
	float fSpeed = 300.0f * g_fDeltaTime * fTimeScale;
	// 총알 초당 이동속도 : 600
	float bulletSpeed = 600.0f * g_fDeltaTime * fTimeScale;

	if (GetAsyncKeyState(VK_SPACE))
	{
		//총알 발사
		BULLET tBullet;

		tBullet.tSphere.x = g_tPlayerRC.right + 50.0f;
		tBullet.tSphere.y = g_tPlayerRC.top + 50.0f;
		tBullet.tSphere.radius = 25.0f;
		tBullet.fDist = 0.f;
		tBullet.fLimitDist = 100.f;

		g_PlayerBulletList.push_back(tBullet);
	}

	if (GetAsyncKeyState(VK_LBUTTON))
	{
		//마우스 위치를 얻어온다.
		POINT ptMouse;
		
		// 마우스 위치를 얻어온다.
		// 마우스 위치는 스크린 좌표 기준으로 받아온다.
		GetCursorPos(&ptMouse);

		// 스크린 좌표를 클라이언트 좌표로 변환한다.
		ScreenToClient(g_hWnd, &ptMouse);


		// 플레이어와 충돌처리를 한다.
		if (g_tPlayerRC.left <= ptMouse.x && ptMouse.x <= g_tPlayerRC.right &&
			g_tPlayerRC.top <= ptMouse.y && ptMouse.y <= g_tPlayerRC.bottom)
		{
			MessageBox(NULL, L"플레이어 클릭", L"마우스 클릭", MB_OK);
		}
		
		float fX = g_tMonster.tSphere.x - ptMouse.x;
		float fY = g_tMonster.tSphere.y - ptMouse.y;
		float fDist = sqrtf(fX * fX + fY * fY);
		if (g_tMonster.tSphere.radius >= fDist)
		{
			MessageBox(NULL, L"몬스터 클릭", L"마우스 클릭", MB_OK);
		}
	}


	// 클라이언트창의 영역만 계산하여 가져온다.
	// 현재 클라이언트의 창을 가져온다.
	RECT rcWindow;
	GetClientRect(g_hWnd, &rcWindow);
	SetRect(&rcWindow, 0, 0, 800, 600);

	//플레이어 이동
	if (GetAsyncKeyState('D'))
	{
		if (g_tPlayerRC.right <= rcWindow.right)
		{
			g_tPlayerRC.left += fSpeed;
			g_tPlayerRC.right += fSpeed;
		}
	}

	if (GetAsyncKeyState('A'))
	{	
		if (g_tPlayerRC.left >= rcWindow.left)
		{
			g_tPlayerRC.left -= fSpeed;
			g_tPlayerRC.right -= fSpeed;
		}
	}

	if (GetAsyncKeyState('S'))
	{
		if (g_tPlayerRC.bottom <= rcWindow.bottom)
		{
			g_tPlayerRC.top += fSpeed;
			g_tPlayerRC.bottom += fSpeed;
		}
	}

	if (GetAsyncKeyState('W'))
	{
		if (g_tPlayerRC.top >= rcWindow.top)
		{
			g_tPlayerRC.top -= fSpeed;
			g_tPlayerRC.bottom -= fSpeed;
		}
	}

	//몬스터 이동
	g_tMonster.tSphere.y += g_tMonster.fSpeed * g_fDeltaTime * fTimeScale * g_tMonster.iDir;

	if (g_tMonster.tSphere.y + g_tMonster.tSphere.radius >= 600)
	{
		g_tMonster.iDir = MD_BACK;
		g_tMonster.tSphere.y = 550;
	}
	else if (g_tMonster.tSphere.y - g_tMonster.tSphere.radius <= 0)
	{
		g_tMonster.iDir = MD_FRONT;
		g_tMonster.tSphere.y = 50;
	}


	// 몬스터 총알 발사 로직
	g_tMonster.fTime += g_fDeltaTime * fTimeScale; 

	// 1초가 지나면 총알을 발사함(초당 공격속도)
	if (g_tMonster.fTime >= g_tMonster.fLimitTime)
	{
		g_tMonster.fTime -= g_tMonster.fLimitTime;

		//총알 발사
		BULLET tBullet = {};

		tBullet.tSphere.x = g_tMonster.tSphere.x - g_tMonster.tSphere.radius - 25.0f;
		tBullet.tSphere.y = g_tMonster.tSphere.y;
		tBullet.tSphere.radius = 25.0f;
		tBullet.fDist = 0.f;
		tBullet.fLimitDist = 800.f;

		g_MonsterBulletList.push_back(tBullet);
	}


	//플레이어의 총알 이동
	list<BULLET>::iterator iter; //총알 발사 list의 반복자(iterator)
	list<BULLET>::iterator iterEnd = g_PlayerBulletList.end(); //플레이어 총알 list의 마지막

	//플레이어 총알 속도 부여
	for (iter = g_PlayerBulletList.begin(); iter != iterEnd; )
	{
		(*iter).tSphere.x += bulletSpeed;
		(*iter).fDist += fSpeed;
		
		float fX = (*iter).tSphere.x - g_tMonster.tSphere.x;
		float fY = (*iter).tSphere.y - g_tMonster.tSphere.y;
		float fDist = sqrtf(fX * fX + fY * fY);

		// 원 vs 원 충돌
		if (fDist <= (*iter).tSphere.radius + g_tMonster.tSphere.radius)
		{
			iter = g_PlayerBulletList.erase(iter);
			iterEnd = g_PlayerBulletList.end();
		}


		//if ((*iter).tSphere.x - (*iter).tSphere.radius >= 800)
		//{
		//	iter = g_PlayerBulletList.erase(iter);
		//	iterEnd = g_PlayerBulletList.end();
		//}
		else if ((*iter).fDist >= (*iter).fLimitDist)
		{
			iter = g_PlayerBulletList.erase(iter);
			iterEnd = g_PlayerBulletList.end();
		}
		else
		{
			++iter;
		}
	}

	iterEnd = g_MonsterBulletList.end(); //몬스터 총알의 list의 마지막

	//몬스터 총알 속도 부여
	for (iter = g_MonsterBulletList.begin(); iter != iterEnd; )
	{
		(*iter).tSphere.x -= bulletSpeed;
		(*iter).fDist += fSpeed;

		if ((*iter).tSphere.x + (*iter).tSphere.radius <= 0)
		{
			iter = g_MonsterBulletList.erase(iter);
			iterEnd = g_MonsterBulletList.end();
		}
		else if ((*iter).fDist >= (*iter).fLimitDist)
		{
			iter = g_MonsterBulletList.erase(iter);
			iterEnd = g_MonsterBulletList.end();
		}
		// 사각형 vs 사각형 충돌
		//else if (g_tPlayerRC.left <= (*iter).rc.right && (*iter).rc.left <= g_tPlayerRC.right &&
		//	g_tPlayerRC.top <= (*iter).rc.bottom && (*iter).rc.top <= g_tPlayerRC.bottom)
		//{
		//	iter = g_MonsterBulletList.erase(iter);
		//	iterEnd = g_MonsterBulletList.end();
		//}
		else
		{
			++iter;
		}
	}


	//좌표 이동한 사각형의 잔상이 남지 않도록 화면을 덮는다.
	//Rectangle(g_hDC, 0.0f, 0.0f, 800.0f, 600.0f);

	//몬스터(사각형) 출력
	Ellipse(g_hDC, g_tMonster.tSphere.x - g_tMonster.tSphere.radius,
		g_tMonster.tSphere.y - g_tMonster.tSphere.radius,
		g_tMonster.tSphere.x + g_tMonster.tSphere.radius,
		g_tMonster.tSphere.y + g_tMonster.tSphere.radius);

	//플레이어(사각형) 출력
	Rectangle(g_hDC, g_tPlayerRC.left, g_tPlayerRC.top
		, g_tPlayerRC.right, g_tPlayerRC.bottom);


	iterEnd = g_PlayerBulletList.end(); //플레이어 총알의 list의 마지막
	for (iter = g_PlayerBulletList.begin(); iter != iterEnd; ++iter)
	{
		//총알 출력
		Ellipse(g_hDC, (*iter).tSphere.x - (*iter).tSphere.radius,
			(*iter).tSphere.y - (*iter).tSphere.radius,
			(*iter).tSphere.x + (*iter).tSphere.radius,
			(*iter).tSphere.y + (*iter).tSphere.radius);
	}


	iterEnd = g_MonsterBulletList.end(); //몬스터 총알의 list의 마지막
	for (iter = g_MonsterBulletList.begin(); iter != iterEnd; ++iter)
	{
		//총알 출력
		Ellipse(g_hDC, (*iter).tSphere.x - (*iter).tSphere.radius,
			(*iter).tSphere.y - (*iter).tSphere.radius,
			(*iter).tSphere.x + (*iter).tSphere.radius,
			(*iter).tSphere.y + (*iter).tSphere.radius);
	}
}
