// DemoApp.cpp : 애플리케이션에 대한 진입점을 정의합니다.

#include "../framework.h"
#include "DemoApp.h"
#include "../../GameApp_v01/Time/TimeSystem.h"
#include "../../GameApp_v01/Input/KeyBoard/Keyboard_InputSystem.h"


DemoApp::DemoApp(HINSTANCE hInstance) : GameApp::GameApp(hInstance)
{
	appName = TEXT("채린's D2D");

	wcex.lpszClassName = appName;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(107));	// 타이틀 바 등 기본 아이콘
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(108));	// 작업 표시줄 등 쓰이는 작은 아이콘
}

DemoApp::~DemoApp()
{
}

bool DemoApp::Initialize(UINT _width, UINT _height)
{
	bool bRet = GameApp::Initialize(_width, _height);
	if (!bRet) { return false; }

	ShowWindow(s_hWnd, SW_SHOWNORMAL);	//  일반적으로 윈도우를 일반 크기와 위치로 보여줄 때 사용 (SW_SHOWMAXIMIZED => 윈도우 최대화)
	UpdateWindow(s_hWnd);

	// 디버그용 콘솔창 생성 *Debug*
	bool bUseConsole = true;	// 이거 true로 바꾸면 콘솔창 뜸.
	if (bUseConsole)
	{
		AllocConsole();
		FILE* _tempFile;
		freopen_s(&_tempFile, "CONOUT$", "w", stdout);
	}
	
	return true;
}

void DemoApp::Update(float _deltaTime)
{
	__super::Update(_deltaTime);

}

void DemoApp::Render()
{
	__super::Render();
}

void DemoApp::UnInitialize()
{
	__super::UnInitialize();
}
