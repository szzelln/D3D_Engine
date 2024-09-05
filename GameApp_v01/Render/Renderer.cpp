#include "../pch.h"
#include "Renderer.h"
#include "../Engine/GameApp.h"

#define USE_FLIPMODE 1			// 0 : BitBlt모드  1 : Flip모드

Renderer* Renderer::m_pInstance = nullptr;

Renderer::Renderer() : color(1.0f, 0.7f, 0.6f, 1.0f)
{
	m_pInstance = this;
}

Renderer::~Renderer()
{
	UnInit();
}

void Renderer::Init()
{
	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();
}

void Renderer::RenderBegin()
{
#if USE_FLIPMODE==1
	// Flip모드에서는 매프레임 설정 필요 (Bitblt모드는 1번만 하면 되서 Init에서 해준다.)
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
#endif

	// 화면 칠하기.
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);

	// viewport정보(화면 크기 정보)를 기입함
	m_pDeviceContext->RSSetViewports(1, &m_viewport);
}

void Renderer::RenderEnd()
{
	// 백 버퍼 모두 그리고, 백 버퍼에 그린 이미지를 프론트 버퍼로 전송 (=> 화면에 출력)
	HR_T(m_pSwapChain->Present(1, 0));
}

void Renderer::UnInit()
{
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDevice);
}

void Renderer::CreateDeviceAndSwapChain()
{
	// 결과값.
	HRESULT hr = 0;

	// 스왑체인 속성 설정 구조체 생성.
	DXGI_SWAP_CHAIN_DESC swapDesc = {};

	ZeroMemory(&swapDesc, sizeof(swapDesc));

#if USE_FLIPMODE==1
	swapDesc.BufferCount = 2;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
#else
	swapDesc.BufferCount = 1;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	//기본값 0
#endif
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = GameApp::GethWnd();	// 스왑체인 출력할 창 핸들 값.
	swapDesc.Windowed = true;		// 창 모드 여부 설정.
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	
	// 백버퍼(텍스처)의 가로/세로 크기 설정.
	swapDesc.BufferDesc.Width = GameApp::s_ClientSize.width;
	swapDesc.BufferDesc.Height = GameApp::s_ClientSize.height;
	
	// 화면 주사율 설정.
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	
	// 샘플링 관련 설정.
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;


	UINT creationFlags = 0;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// 1. 장치 생성.   2.스왑체인 생성. 3.장치 컨텍스트 생성.
	HR_T(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL,
		D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext));
}

void Renderer::CreateRenderTargetView()
{
	// 4. 렌더타겟뷰 생성.  (백버퍼를 이용하는 렌더타겟뷰)	
	ID3D11Texture2D* pBackBufferTexture = nullptr;
	HR_T(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture));
	HR_T(m_pDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &m_pRenderTargetView));  // 텍스처는 내부 참조 증가
	SAFE_RELEASE(pBackBufferTexture);	//외부 참조 카운트를 감소시킨다.

#if USE_FLIPMODE==0
	// 렌더 타겟을 최종 출력 파이프라인에 바인딩합니다.
	// FlipMode가 아닐때는 최초 한번만 설정하면 된다.
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
#endif
}

void Renderer::SetViewport()
{
	// 5. 뷰포트 설정.
	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = static_cast<float>(GameApp::s_ClientSize.width);
	m_viewport.Height = static_cast<float>(GameApp::s_ClientSize.height);
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
}