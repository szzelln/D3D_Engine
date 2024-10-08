#include "framework.h"
#include "DemoApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    DemoApp app(hInstance);
    app.Initialize(1280,720);
    app.Loop();
    app.UnInitialize();
}