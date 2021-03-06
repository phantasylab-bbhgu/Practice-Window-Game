// Practice_Window_Game.cpp: 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Practice_Window_Game.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


struct _tagArea
{
	bool bStart;
	POINT ptStart;
	POINT ptEnd;
};

_tagArea g_tArea;

//Win32 Entry Function (진입점 함수 - Main 함수)
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
    LoadStringW(hInstance, IDC_PRACTICEWINDOWGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	//LoadAccelerators : 윈도우 리소스에서 제공하는 단축키 모음, 성능 떨어짐
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PRACTICEWINDOWGAME));

	//윈도우 메시지 구조체(어디 윈도우의 메시지인가?)
    MSG msg;

    // 기본 메시지 루프입니다.
	// 운영체제가 기본적으로 메시지 큐를 제공
	// 윈도우의 메시지 루프
	// 메시지가 들어올때까지 계속 돈다
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
			//메세지 큐에 메시지가 들어왔다.

			//내부적으로 어떤 이벤트(메시지)인지 구분해줌
            TranslateMessage(&msg);

			//
            DispatchMessage(&msg);
        }
    }

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PRACTICEWINDOWGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_PRACTICEWINDOWGAME);
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

   //1번째 CW_USEDEFAULT, 그 다음 인수 : 윈도우 좌표계는 좌상단이 (0,0) 스크린 윈도우를 어디 좌표에서 만들것인가?
   //2번째 CW_USEDEFAULT, 그 다음 인수 : 윈도우 크기를 X,Y 크기를 얼마나 줄것인가?
   //hInstance : 윈도우창 어떤 프로그램 소속인가?

   if (!hWnd)
   {
      return FALSE;
   }

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
	// 마우스 왼쪽 버튼을 눌렀을때
	case WM_LBUTTONDOWN:
	{
		// 마우스 위치는 lParam에 들어오게 되는데 16비트로 쪼개서 x, y값이
		// 32비트 변수에 들어오게 된다.
		// LOWRD, HIWORD 매크로를 이용해서 하위, 상위 16비트의 값을 얻어올 수 있다.
		if (!g_tArea.bStart)
		{
			g_tArea.bStart = true;
			// 만약 lParam이 0x12345678 & 0x0000ffff 일때의 결과는?
			// -> 0x00005678
			// 만약 lParam이 0x12345678 >> 16 일때의 결과는?(오른쪽으로 16비트 밀었음)
			// -> 0x00001234

			//0x12345678에서
			//0x00005678 = x 값 나타냄(하위 16bit)
			//0x00001234 = y 값 나타냄(상위 16bit)
			g_tArea.ptStart.x = lParam & 0x0000ffff;
			g_tArea.ptStart.y = lParam >> 16;

			//처음 누를때 endPoint를 갱신해줌
			g_tArea.ptEnd = g_tArea.ptStart;

			// InvalidateRect 함수는 강제로 WM_PAINT 메시지를 호출해주는 함수
			// 1번 인자는 윈도우 핸들이 들어간다.
			// 2번 인자는 초기화할 영역이 들어가는데, null을 넣어줄 경우 전체 화면을 대상으로 갱신
			// 3번 인자는 TRUE일 경우 현재 화면을 지우고 갱신한다. FALSE 할 경우 현재 화면을 안지우고 갱신
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	}
	break;
		// 키가 눌러졌을때 들어오는 메시지
	case WM_KEYDOWN:
		// 이 메시지가 들어올 경우 wParam에 어떤 키를 눌렀는지가 들어온다.
		// 마우스는 따로
		switch (wParam)
		{
			//ESC를 눌렀을 경우 윈도우 창 종료
			case VK_ESCAPE:
				DestroyWindow(hWnd);
				break;
		}
		break;
		// 마우스가 움직일 때 들어오는 메시지
	case WM_MOUSEMOVE:
		if (g_tArea.bStart)
		{
			g_tArea.ptEnd.x = lParam & 0x0000ffff;
			g_tArea.ptEnd.y = lParam >> 16;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
		// 마우스 왼쪽을 눌렀다 뗐을 때 발생하는 메시지
	case WM_LBUTTONUP:
		if (g_tArea.bStart)
		{
			g_tArea.bStart = false;
			g_tArea.ptEnd.x = lParam & 0x0000ffff;
			g_tArea.ptEnd.y = lParam >> 16;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
    case WM_PAINT: //화면에 무언가를 그리거나, 윈도우에 갱신될때의 메시지
        {
            PAINTSTRUCT ps;
			//dc : 화면에 그리기위한 도구, 클라이언트 영역에 그리기 위한 도구
            HDC hdc = BeginPaint(hWnd, &ps);

            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.  

			// 유니코드 문자열은 "" 앞에 L을 붙여서 L"" 로 해줄 수 있다.
			// 아니면 TEXT 매크로를 이용한다.
			// TEXT 매크로는 자동으로 멀티바이트, 유니코드를 구분한다.
			// c는 글자 count
			TextOut(hdc, 100, 100, TEXT("win32"), 5);
			TextOut(hdc, 200, 200, TEXT("win32"), 5);
			TextOut(hdc, 300, 300, TEXT("win32"), 5);
			//TextOut(hdc, 100, 100, L"Test", 5);

			TCHAR strMouse[64] = {};

			// wsprintf : 유니코드 문자열을 만들어주는 함수.
			// %d 에는 정수가 대입
			wsprintf(strMouse, TEXT("Start = x : %d y : %d"), g_tArea.ptStart.x, g_tArea.ptStart.y);

			// lstrlen : 유니코드 문자열의 길이를 구해주는 함수
			TextOut(hdc, 600, 30, strMouse, lstrlen(strMouse));

			// 사각형 그리기
			// 1번째 인자 : 사각형 어디에 그릴래?
		    // 2,3번째 인자 : left Top
			// 4,5번째 인자 : right bottom
			Rectangle(hdc, 30, 30, 50, 50);

			
			// 원 그리기
			// 1번째 인자 : 원을 어디에 그릴래?
			// 2,3번째 인자 : left Top
			// 4,5번째 인자 : right bottom
			Ellipse(hdc, 60, 60, 100, 100);


			// 선 그리기(시작점)
			// 1번째 인자 : 어디에 선 그림?
			// 2번째 3번째 인자 : 선 시작점 x,y
			MoveToEx(hdc, 200, 100, NULL);

			// 선 그리기(마지막)
			// 1번째 인자 : 어디에 선 그림?
			// 2번째, 3번째 인자 : 선 도착점 x,y
			LineTo(hdc, 100, 200);

			// 선 그리기(마지막)을 한번 더 추가되면
			// 이전의 LineTo 함수는 시작점이 된다.
			LineTo(hdc, 400, 500);

			// 선을 새로 그리고 싶으면
			// MoveToEx 함수를 다시 써서 새로 시작해야 한다.


			if (g_tArea.bStart)
			{
				Rectangle(hdc, g_tArea.ptStart.x, g_tArea.ptStart.y, g_tArea.ptEnd.x, g_tArea.ptEnd.y);
			}

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY: //윈도우가 종료될때
        PostQuitMessage(0); //윈도우 창을 아애 끝내버리는 함수
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam); //기본적으로 제공하는 윈도우 프로시저(윈도우의 기본기능으로 처리)
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
