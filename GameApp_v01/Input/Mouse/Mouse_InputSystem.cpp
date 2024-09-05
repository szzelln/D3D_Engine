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

	SetCursorPos((int)m_curMouse.pos.x, (int)m_curMouse.pos.y);				// ���콺 ȭ�� ��� �α�
}

void Mouse_InputSystem::Update()
{
	if (GameApp::GethWnd() != ::GetActiveWindow()) { return; }			// HWND�� �߸� �޾ƿ��� return

	POINT pt;
	if (GetCursorPos(&pt) == false) { return; }				// ���콺 ��ǥ ���޾ƿ��� return
	if (ScreenToClient(GameApp::GethWnd(), &pt) == false) { return; }	// ȭ�� ��ǥ->Ŭ���̾�Ʈ ���� ��ǥ�� ��ȯ �����ϸ� return

	// ���콺 ��ǥ�� ȭ�� ������ ����� �ʵ��� ��ǥ ����
	if (m_screenRect.left > pt.x) { return; }
	if (m_screenRect.right < pt.x) { return; }
	if (m_screenRect.top > pt.y) { return; }
	if (m_screenRect.bottom < pt.y) { return; }

	// ���� ������ �����ϸ� �Ʒ� �ڵ� ������.
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