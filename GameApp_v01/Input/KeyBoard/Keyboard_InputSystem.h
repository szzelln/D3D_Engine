#pragma once
#include "../../Tool/TSingleton.h"

class Keyboard_InputSystem : public TSingleton<Keyboard_InputSystem>
{
	friend class TSingleton<Keyboard_InputSystem>;

private:
	Keyboard_InputSystem() = default;
	virtual ~Keyboard_InputSystem() = default;

	Keyboard_InputSystem(const Keyboard_InputSystem&) = delete;
	Keyboard_InputSystem& operator=(const Keyboard_InputSystem&) = delete;

public:
	void Init();
	void Update();

	// 키보드 상태 체크
	void KeyDown(UINT _key);
	void KeyUp(UINT _key);

	// 키보드 변수 값 확인용 함수(Get함수 느낌)
	bool IsKey(UINT _key);
	bool IsKeyDown(UINT _key);
	bool IsKeyUp(UINT _key);

private:
	bool isKey[256] = { 0, };
	bool isKeyDown[256] = { 0, };
	bool isKeyUp[256] = { 0, };
};

