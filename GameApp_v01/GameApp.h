#pragma once
#include "pch.h"
#include "Renderer.h"

class GameApp
{
public:
	GameApp(HINSTANCE hInstance);
	virtual ~GameApp();

public:
	static void PlaceInCenterOfScreen(HWND window, DWORD style, DWORD exStyle);
	static LRESULT CALLBACK	WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual bool Initialize(UINT _width, UINT _height);
	void Loop();
	virtual void Update(float _deltaTime);
	virtual void Render();
	virtual void UnInitialize();

	static HWND GethWnd() { return s_hWnd; }

	void FrameRateCalculate();		// FPS 와 SPF 계산

public:
	static D2D_SIZE_U s_MoniterSize;	// 모니터 해상도 사이즈 구조체
	static D2D_SIZE_U s_ClientSize;		// 윈도우 사이즈 구조체

protected:
	static HWND s_hWnd;		//자주필요하니 포인터 간접접근을 피하기위해 정적멤버로 만들었다.

	WNDCLASSEX wcex = {};
	const TCHAR* appName = nullptr;

	Renderer m_Renderer;

private:
	HINSTANCE m_hInstance = nullptr;
};
