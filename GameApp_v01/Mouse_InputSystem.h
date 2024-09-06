#pragma once
#include "TSingleton.h"

struct MouseState
{
	MouseState()
	{
		pos.x = 0.0f;
		pos.y = 0.0f;
		wheel = 0.0f;
		left = false;
		right = false;
		middle = false;
	}

	D2D1_POINT_2F pos;
	float wheel;

	bool left;
	bool right;
	bool middle;
};

class Mouse_InputSystem : public TSingleton<Mouse_InputSystem>
{
	friend class TSingleton<Mouse_InputSystem>;

private:
	Mouse_InputSystem() = default;
	virtual ~Mouse_InputSystem() = default;

	Mouse_InputSystem(const Mouse_InputSystem&) = delete;
	Mouse_InputSystem& operator=(const Mouse_InputSystem&) = delete;

public:
	void Init();
	void Update();

	const D2D1_POINT_2F& GetCurMousePos() const;

	bool LMouseClicked();
	bool LMouseClicked(D2D1_POINT_2F& _objPos);
	bool LMouseClickUp();
	bool LMouseClickUp(D2D1_POINT_2F& _objPos);

	bool RMouseClicked(D2D1_POINT_2F& _objPos);
	bool RMouseClickUp(D2D1_POINT_2F& _objPos);

private:
	MouseState m_curMouse;
	MouseState m_prevMouse;

	RECT m_screenRect = { 0 };	// 윈도우 사이즈 구조체(상하좌우)

};

