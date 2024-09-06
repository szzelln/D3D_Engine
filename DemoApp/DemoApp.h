#pragma once

#include "resource.h"
#include "../GameApp_v01/GameApp.h"

class DemoApp : public GameApp
{
public:
	DemoApp(HINSTANCE hInstance);
	virtual ~DemoApp();

public:
	virtual bool Initialize(UINT _width, UINT _height) override;
	virtual void Update(float _deltaTime) override;
	virtual void Render() override;
	virtual void UnInitialize() override;

private:

};