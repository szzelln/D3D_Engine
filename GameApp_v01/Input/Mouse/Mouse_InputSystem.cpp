#include "../../pch.h"
#include "Mouse_InputSystem.h"
#include "../../Engine/GameApp.h"

void Mouse_InputSystem::Init()
{
	GetClientRect(GameApp::GethWnd(), &m_screenRect);

	m_curMouse.pos.x = static_cast<float>(GameApp::s_MoniterSize.width) / 2.0f;
	m_curMouse.pos.y = static_cast<float>(GameApp::s_MoniterSize.height) / 2.0f;
	m_curMouse.wheel = 0.0f;

	m_curMouse.left = false;
	m_curMouse.right = false;
	m_curMouse.middle = false;

	m_prevMouse = m_curMouse;

	SetCursorPos((int)m_curMouse.pos.x, (int)m_curMouse.pos.y);				// 마우스 화면 가운데 두기
}

void Mouse_InputSystem::Update()
{
	if (GameApp::GethWnd() != ::GetActiveWindow()) { return; }			// HWND를 잘못 받아오면 return

	POINT pt;
	if (GetCursorPos(&pt) == false) { return; }				// 마우스 좌표 못받아오면 return
	if (ScreenToClient(GameApp::GethWnd(), &pt) == false) { return; }	// 화면 좌표->클라이언트 영역 좌표로 변환 실패하면 return

	// 마우스 좌표가 화면 영역을 벗어나지 않도록 좌표 제한
	if (m_screenRect.left > pt.x) { return; }
	if (m_screenRect.right < pt.x) { return; }
	if (m_screenRect.top > pt.y) { return; }
	if (m_screenRect.bottom < pt.y) { return; }

	// 위의 내용을 만족하면 아래 코드 실행함.
	m_prevMouse = m_curMouse;

	m_curMouse.pos.x = pt.x;
	m_curMouse.pos.y = pt.y;
	m_curMouse.wheel = 0;

	m_curMouse.left = (GetKeyState(VK_LBUTTON) & 0x8000) != 0;
	m_curMouse.right = (GetKeyState(VK_RBUTTON) & 0x8000) != 0;
	m_curMouse.middle = (GetKeyState(VK_MBUTTON) & 0x8000) != 0;
}

const D2D1_POINT_2F& Mouse_InputSystem::GetCurMousePos() const
{
	return m_curMouse.pos;
}

bool Mouse_InputSystem::LMouseClicked()
{
	if (m_curMouse.left == true)
	{
		return true;
	}

	return false;
}

bool Mouse_InputSystem::LMouseClicked(D2D1_POINT_2F& _objPos)
{
	if (m_curMouse.left == true)
	{
		_objPos.x = m_curMouse.pos.x;
		_objPos.y = m_curMouse.pos.y;

		return true;
	}

	return false;
}

bool Mouse_InputSystem::LMouseClickUp()
{
	if (m_prevMouse.left == true && m_curMouse.left == false)
	{
		return true;
	}

	return false;
}

bool Mouse_InputSystem::LMouseClickUp(D2D1_POINT_2F& _objPos)
{
	if (m_prevMouse.left == true && m_curMouse.left == false)
	{
		_objPos.x = m_curMouse.pos.x;
		_objPos.y = m_curMouse.pos.y;

		return true;
	}

	return false;
}

bool Mouse_InputSystem::RMouseClicked(D2D1_POINT_2F& _objPos)
{
	if (m_curMouse.right == true)
	{
		_objPos.x = m_curMouse.pos.x;
		_objPos.y = m_curMouse.pos.y;

		return true;
	}

	return false;
}

bool Mouse_InputSystem::RMouseClickUp(D2D1_POINT_2F& _objPos)
{
	if (m_prevMouse.right == true && m_curMouse.right == false)
	{
		_objPos.x = m_curMouse.pos.x;
		_objPos.y = m_curMouse.pos.y;

		return true;
	}

	return false;
}