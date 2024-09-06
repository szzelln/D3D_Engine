// GameApp.cpp : ���� ���̺귯���� ���� �Լ��� �����մϴ�.
//
#include "pch.h"
#include "GameApp.h"
#include "Keyboard_InputSystem.h"
#include "Mouse_InputSystem.h"
#include "TimeSystem.h"


HWND GameApp::s_hWnd = nullptr;
D2D_SIZE_U GameApp::s_MoniterSize = { 0 };
D2D_SIZE_U GameApp::s_ClientSize = { 0 };

GameApp::GameApp(HINSTANCE hInstance) : m_hInstance(hInstance)
{
	// â Ŭ���� ��� => �Լ�: MyRegisterClass()
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstance;
	wcex.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));	// WHITE_BRUSH
}

GameApp::~GameApp()
{
}

void GameApp::PlaceInCenterOfScreen(HWND window, DWORD style, DWORD exStyle)
{
	s_MoniterSize.width = (UINT)GetSystemMetrics(SM_CXSCREEN);
	s_MoniterSize.height = (UINT)GetSystemMetrics(SM_CYSCREEN);

	RECT clientRect;
	GetClientRect(window, &clientRect);

	s_ClientSize.width = clientRect.right - clientRect.left;
	s_ClientSize.height = clientRect.bottom - clientRect.top;

	SetWindowPos(window, NULL,
		s_MoniterSize.width / 2 - s_ClientSize.width / 2,
		s_MoniterSize.height / 2 - s_ClientSize.height / 2,
		s_ClientSize.width, s_ClientSize.height, 0
	);
}

LRESULT GameApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		PlaceInCenterOfScreen(hWnd, WS_OVERLAPPEDWINDOW, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

bool GameApp::Initialize(UINT _width, UINT _height)
{
	if (!RegisterClassExW(&wcex))
	{
		MessageBox(NULL, L"������ ���� ����", L"����", MB_OK | MB_ICONERROR);
		return 1;
	}

	// Step 2: ������ â ����� (InitInstance �Լ�)
	s_hWnd = CreateWindow(appName, appName, WS_OVERLAPPED | WS_SYSMENU,
		0, 0, _width, _height, NULL, NULL, m_hInstance, NULL);

	if (!s_hWnd)
	{
		MessageBox(NULL, L"������ ���� ����", L"����", MB_OK | MB_ICONERROR);
		return 1;
	}

	// Renderer �ʱ�ȭ
	m_Renderer.Init();
	m_Renderer.InitScene();

	//TimeSystem
	TimeSystem::GetInstance()->Init();

	//InputSystem
	Keyboard_InputSystem::GetInstance()->Init();
	Mouse_InputSystem::GetInstance()->Init();
	
	return true;
}

void GameApp::Loop()
{
	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			if (msg.message == WM_KEYDOWN)
			{
				Keyboard_InputSystem::GetInstance()->KeyDown(msg.wParam);
			}

			if (msg.message == WM_KEYUP)
			{
				Keyboard_InputSystem::GetInstance()->KeyUp(msg.wParam);
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			TimeSystem::GetInstance()->Update();
			Update(TimeSystem::GetInstance()->GetfDeltaTime());
			Render();
		}
	}
}

void GameApp::Update(float _deltaTime)
{
	FrameRateCalculate();

	Mouse_InputSystem::GetInstance()->Update();
	Keyboard_InputSystem::GetInstance()->Update();
}

void GameApp::Render()
{
	// ���� ����
	m_Renderer.RenderBegin();

	// ����
	m_Renderer.RenderScene();

	//���� ��
	m_Renderer.RenderEnd();
}

void GameApp::UnInitialize()
{
	// InputSystem �ı�
	Keyboard_InputSystem::DestroyInstance();
	Mouse_InputSystem::DestroyInstance();

	// TimeSystem �ı�
	TimeSystem::DestroyInstance();
}

void GameApp::FrameRateCalculate()
{
	static int frame = 0;
	static float s_fTimeElapsed = 0;
	frame++;

	s_fTimeElapsed += TimeSystem::GetInstance()->GetfDeltaTime();

	if (s_fTimeElapsed >= 1.0f)
	{
		wchar_t szBuffer[255] = {};
		swprintf_s(szBuffer, L"FPS : %d", frame);
		SetWindowText(s_hWnd, szBuffer);	// ������ Ÿ��Ʋ �ٿ� ���

		s_fTimeElapsed -= 1.0f;
		frame = 0;
	}


}
