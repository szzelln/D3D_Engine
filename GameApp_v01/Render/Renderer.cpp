#include "../pch.h"
#include "Renderer.h"
#include "../Engine/GameApp.h"

#define USE_FLIPMODE 1			// 0 : BitBlt���  1 : Flip���

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
	// Flip��忡���� �������� ���� �ʿ� (Bitblt���� 1���� �ϸ� �Ǽ� Init���� ���ش�.)
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
#endif

	// ȭ�� ĥ�ϱ�.
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);

	// viewport����(ȭ�� ũ�� ����)�� ������
	m_pDeviceContext->RSSetViewports(1, &m_viewport);
}

void Renderer::RenderEnd()
{
	// �� ���� ��� �׸���, �� ���ۿ� �׸� �̹����� ����Ʈ ���۷� ���� (=> ȭ�鿡 ���)
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
	// �����.
	HRESULT hr = 0;

	// ����ü�� �Ӽ� ���� ����ü ����.
	DXGI_SWAP_CHAIN_DESC swapDesc = {};

	ZeroMemory(&swapDesc, sizeof(swapDesc));

#if USE_FLIPMODE==1
	swapDesc.BufferCount = 2;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
#else
	swapDesc.BufferCount = 1;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	//�⺻�� 0
#endif
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = GameApp::GethWnd();	// ����ü�� ����� â �ڵ� ��.
	swapDesc.Windowed = true;		// â ��� ���� ����.
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	
	// �����(�ؽ�ó)�� ����/���� ũ�� ����.
	swapDesc.BufferDesc.Width = GameApp::s_ClientSize.width;
	swapDesc.BufferDesc.Height = GameApp::s_ClientSize.height;
	
	// ȭ�� �ֻ��� ����.
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	
	// ���ø� ���� ����.
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;


	UINT creationFlags = 0;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// 1. ��ġ ����.   2.����ü�� ����. 3.��ġ ���ؽ�Ʈ ����.
	HR_T(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL,
		D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext));
}

void Renderer::CreateRenderTargetView()
{
	// 4. ����Ÿ�ٺ� ����.  (����۸� �̿��ϴ� ����Ÿ�ٺ�)	
	ID3D11Texture2D* pBackBufferTexture = nullptr;
	HR_T(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture));
	HR_T(m_pDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &m_pRenderTargetView));  // �ؽ�ó�� ���� ���� ����
	SAFE_RELEASE(pBackBufferTexture);	//�ܺ� ���� ī��Ʈ�� ���ҽ�Ų��.

#if USE_FLIPMODE==0
	// ���� Ÿ���� ���� ��� ���������ο� ���ε��մϴ�.
	// FlipMode�� �ƴҶ��� ���� �ѹ��� �����ϸ� �ȴ�.
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
#endif
}

void Renderer::SetViewport()
{
	// 5. ����Ʈ ����.
	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = static_cast<float>(GameApp::s_ClientSize.width);
	m_viewport.Height = static_cast<float>(GameApp::s_ClientSize.height);
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
}