#include "pch.h"
#include "Renderer.h"
#include "GameApp.h"

#define USE_FLIPMODE 1			// 0 : BitBlt���  1 : Flip���

Renderer* Renderer::m_pInstance = nullptr;

Renderer::Renderer() : color(1.0f, 0.7f, 0.6f, 1.0f)
{
	m_pInstance = this;
}

Renderer::~Renderer()
{
	UnInitScene();
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


// ===== swapDesc.BufferDesc ���� (����� �Ӽ� ����) =====
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

	// viewport����(ȭ�� ũ�� ����)�� ������
	m_pDeviceContext->RSSetViewports(1, &m_viewport);

	// �ϳ��� â�� �� ���� �ٸ� viewPort�� �����Ǹ�, �� ����Ʈ�� ���� �ٸ� �׷��� ������ �������� �� �ִ�.
	// ex) ���� ���ø����̼ǿ����� �� ���� ȭ��� �̴ϸ��� ������ â�� �������ϴ� ���
}


// ========== �ӽ� (���߿� Scene���� �и� �ؾ���)  ==========
bool Renderer::InitScene()
{
	HRESULT hr = 0; // �����.
	ID3D10Blob* errorMessage = nullptr;	 // ������ ���� �޽����� ����� ����.	

	//1. Render() ���� ���������ο� ���ε��� ���ؽ� ���۹� ���� ���� �غ�
	// ������ VertexShader�� World, View, Projection ��ȯ�� ������� �����Ƿ� 
	// ���� Normalized Device Coordinate(��ǥ��)�� ��ġ�� �����Ѵ�.
	//      /---------------------(1,1,1)   z���� ���̰�
	//     /                      / |   
	// (-1,1,0)----------------(1,1,0)        
	//   |         v1           |   |
	//   |        /   `         |   |       �߾��� (0,0,0)  
	//   |       /  +   `       |   |
	//   |     /         `      |   |
	//	 |   v0-----------v2    |  /
	// (-1,-1,0)-------------(1,-1,0)
	Vertex vertices[] = // x,y,z
	{
		Vector3(-0.5,-0.5,0.5), // v0    
		Vector3(0,0.5,0.5),		// v1    
		Vector3(0.5,-0.5,0.5),	// v2		
	};

	// ���� ����
	D3D11_BUFFER_DESC vbDesc = {};
	m_VertexCount = ARRAYSIZE(vertices);	// ������ ��
	vbDesc.ByteWidth = sizeof(Vertex) * m_VertexCount; // ���ؽ� ������ ũ��(Byte).
	vbDesc.CPUAccessFlags = 0;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // ���� ���۷� ���.
	vbDesc.MiscFlags = 0;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;	// CPU�� ���ٺҰ� ,  GPU���� �б�/���� ������ ���۷� ����.

	// ���� ���� ����.
	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = vertices;	// ���۸� �����Ҷ� ������ �������� �ּ� ���� 
	HR_T(hr = m_pDevice->CreateBuffer(&vbDesc, &vbData, &m_pVertexBuffer));

	// ���ؽ� ���� ���� 
	m_VertextBufferStride = sizeof(Vertex); // ���ؽ� �ϳ��� ũ��
	m_VertextBufferOffset = 0;	// ���ؽ� ���� �ּҿ��� ���� ������ �ּ�

	// 2. Render() ���� ���������ο� ���ε��� InputLayout ���� 	
	D3D11_INPUT_ELEMENT_DESC layout[] =  // ��ǲ ���̾ƿ��� ���ؽ� ���̴��� �Է¹��� �������� ������ �����Ѵ�.
	{// SemanticName , SemanticIndex , Format , InputSlot , AlignedByteOffset , InputSlotClass , InstanceDataStepRate		
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ID3DBlob* vertexShaderBuffer = nullptr;
	HR_T(CompileShaderFromFile(L"BasicVertexShader.hlsl", "main", "vs_4_0", &vertexShaderBuffer));
	HR_T(hr = m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pInputLayout));

	// 3. Render���� ���������ο� ���ε���  ���ؽ� ���̴� ����
	HR_T(m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader));
	SAFE_RELEASE(vertexShaderBuffer);


	// 4. Render���� ���������ο� ���ε��� �ȼ� ���̴� ����
	ID3DBlob* pixelShaderBuffer = nullptr;
	HR_T(CompileShaderFromFile(L"BasicPixelShader.hlsl", "main", "ps_4_0", &pixelShaderBuffer));
	HR_T(m_pDevice->CreatePixelShader(
		pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader));
	SAFE_RELEASE(pixelShaderBuffer);

	return true;
}

void Renderer::RenderScene()
{
	// Draw�迭 �Լ��� ȣ���ϱ����� ������ ���������ο� �ʼ� �������� ������ �ؾ��Ѵ�.	
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // ������ �̾ �׸� ��� ����.
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertextBufferStride, &m_VertextBufferOffset);
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);
	m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

	// Render a triangle	
	m_pDeviceContext->Draw(m_VertexCount, 0);
}

void Renderer::UnInitScene()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pInputLayout);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
}