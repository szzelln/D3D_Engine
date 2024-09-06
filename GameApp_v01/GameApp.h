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

	void FrameRateCalculate();		// FPS �� SPF ���

public:
	static D2D_SIZE_U s_MoniterSize;	// ����� �ػ� ������ ����ü
	static D2D_SIZE_U s_ClientSize;		// ������ ������ ����ü

protected:
	static HWND s_hWnd;		//�����ʿ��ϴ� ������ ���������� ���ϱ����� ��������� �������.

	WNDCLASSEX wcex = {};
	const TCHAR* appName = nullptr;

	Renderer m_Renderer;

private:
	HINSTANCE m_hInstance = nullptr;
};
