#include "pch.h"
#include "Renderer.h"
#include "GameApp.h"

#define USE_FLIPMODE 1			// 0 : BitBlt모드  1 : Flip모드

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
	// Flip모드에서는 매프레임 설정 필요 (Bitblt모드는 1번만 하면 되서 Init에서 해준다.)
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
#endif

	// 화면 칠하기.
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);

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


// ===== swapDesc.BufferDesc 설정 (백버퍼 속성 정의) =====
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

	// viewport정보(화면 크기 정보)를 기입함
	m_pDeviceContext->RSSetViewports(1, &m_viewport);

	// 하나의 창에 두 개의 다른 viewPort가 설정되면, 각 뷰포트에 대해 다른 그래픽 내용을 렌더링할 수 있다.
	// ex) 게임 애플리케이션에서는 주 게임 화면과 미니맵을 동일한 창에 렌더링하는 경우
}


// ========== 임시 (나중에 Scene으로 분리 해야함)  ==========

	// 1. Render() 에서 파이프라인에 바인딩할 버텍스 버퍼및 버퍼 정보 준비
	// 2. Render() 에서 파이프라인에 바인딩할 InputLayout 생성 	
	// 3. Render() 에서 파이프라인에 바인딩할  버텍스 셰이더 생성
	// 4. Render() 에서 파이프라인에 바인딩할 픽셀 셰이더 생성
bool Renderer::InitScene()
{
	HRESULT hr = 0; // 결과값.
	ID3D10Blob* errorMessage = nullptr;	 // 컴파일 에러 메시지가 저장될 버퍼.	

	//1. Render() 에서 파이프라인에 바인딩할 버텍스 버퍼및 버퍼 정보 준비
	// 아직은 VertexShader의 World, View, Projection 변환을 사용하지 않으므로 
	// 직접 Normalized Device Coordinate(좌표계)의 위치로 설정한다.
	//      /---------------------(1,1,1)   z값은 깊이값
	//     /                      / |   
	// (-1,1,0)----------------(1,1,0)        
	//   |         v1           |   |
	//   |        /   `         |   |       중앙이 (0,0,0)  
	//   |       /  +   `       |   |
	//   |     /         `      |   |
	//	 |   v0-----------v2    |  /
	// (-1,-1,0)-------------(1,-1,0)
	Vertex vertices[] = // Vector3(x, y, z), Vector4(R, G, B, A)
	{
		Vertex(Vector3(0.0f,  0.5f, 0.5f), Vector4(0.6f, 0.8f, 0.9f, 1.0f)),	// v0
		Vertex(Vector3(0.5f, -0.5f, 0.5f), Vector4(1.0f, 0.7f, 0.9f, 1.0f)),	// v1
		Vertex(Vector3(-0.5f, -0.5f, 0.5f), Vector4(0.9f, 0.9f, 0.9f, 1.0f))	// v2	
	};

	m_VertexCount = ARRAYSIZE(vertices);	// 정점의 수

	// 버퍼 설정
	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.ByteWidth = sizeof(Vertex) * m_VertexCount;	// 버텍스 버퍼의 크기(Byte).
	vbDesc.Usage = D3D11_USAGE_DEFAULT;	// CPU는 접근불가 ,  GPU에서 읽기/쓰기 가능한 버퍼로 생성.
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// 정점 버퍼로 사용.
	vbDesc.CPUAccessFlags = 0;							// Obj가 실시간으로 움직이면 CPU에 접근해야 되서 0이 아닌 다른 값을 넣어야 한다.
	vbDesc.MiscFlags = 0;

	// 정점 버퍼 생성.
	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = vertices;	// 버퍼를 생성할때 복사할 데이터의 주소 설정 
	HR_T(hr = m_pDevice->CreateBuffer(&vbDesc, &vbData, &m_pVertexBuffer));

	// 버텍스 버퍼 정보 
	m_VertextBufferStride = sizeof(Vertex); // 버텍스 하나의 크기
	m_VertextBufferOffset = 0;	// 버텍스 시작 주소에서 더할 오프셋 주소

	// 2. Render() 에서 파이프라인에 바인딩할 InputLayout 생성 	
	D3D11_INPUT_ELEMENT_DESC layout[] =  // 인풋 레이아웃은 버텍스 쉐이더가 입력받을 데이터의 형식을 지정한다.
	{// SemanticName , SemanticIndex , Format , InputSlot , AlignedByteOffset , InputSlotClass , InstanceDataStepRate		
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },	 // 4byte * 3 = 12byte 다음의 데이터는 12byte 떨어짐.
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 } // 12 대신 D3D11_APPEND_ALIGNED_ELEMENT 사용 가능.
	};

	ID3DBlob* vertexShaderBuffer = nullptr;
	HR_T(CompileShaderFromFile(L"BasicVertexShader.hlsl", "main", "vs_4_0", &vertexShaderBuffer));
	HR_T(hr = m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pInputLayout));

	// 3. Render에서 파이프라인에 바인딩할  버텍스 셰이더 생성
	HR_T(m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader));
	SAFE_RELEASE(vertexShaderBuffer);	// 버퍼 더이상 필요없음.


	// 4. Render에서 파이프라인에 바인딩할 픽셀 셰이더 생성
	ID3DBlob* pixelShaderBuffer = nullptr;
	HR_T(CompileShaderFromFile(L"BasicPixelShader.hlsl", "main", "ps_4_0", &pixelShaderBuffer));
	HR_T(m_pDevice->CreatePixelShader(
		pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader));
	SAFE_RELEASE(pixelShaderBuffer);	// 버퍼 더이상 필요없음.

	return true;
}

void Renderer::RenderScene()
{
	// Draw계열 함수를 호출하기전에 렌더링 파이프라인에 필수 스테이지 설정을 해야한다.	
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 정점을 이어서 그릴 방식 설정.
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