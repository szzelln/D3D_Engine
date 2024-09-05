#include "../../pch.h"
#include "Keyboard_InputSystem.h"

void Keyboard_InputSystem::Init()
{
	for (int i = 0; i < 256; i++)
	{
		isKey[i] = false;
		isKeyDown[i] = false;
		isKeyUp[i] = false;
	}
}

void Keyboard_InputSystem::Update()
{
	for (int i = 0; i < 256; i++)
	{
		isKeyDown[i] = false;
		isKeyUp[i] = false;
	}
}

void Keyboard_InputSystem::KeyDown(UINT _key)
{
	isKey[_key] = true;
	isKeyDown[_key] = true;
}

void Keyboard_InputSystem::KeyUp(UINT _key)
{
	isKey[_key] = false;
	isKeyUp[_key] = true;
}

bool Keyboard_InputSystem::IsKey(UINT _key)
{
	return isKey[_key];
}

bool Keyboard_InputSystem::IsKeyDown(UINT _key)
{
	return isKeyDown[_key];
}

bool Keyboard_InputSystem::IsKeyUp(UINT _key)
{
	return isKeyUp[_key];
}