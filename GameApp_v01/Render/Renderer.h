#pragma once
class Renderer
{
public:
	Renderer();
	~Renderer();

public:
	void Init();
	void RenderBegin();
	void RenderEnd();
	void UnInit();

private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewport();					// viewport 정보 설정

public:
	static Renderer* m_pInstance;

private:
	DirectX::SimpleMath::Color color;	// 초기화 색상

	// 렌더링 파이프라인을 구성하는 필수 객체의 인터페이스 ( 뎊스 스텐실 뷰도 있지만 아직 사용하지 않는다.)
	ID3D11Device* m_pDevice = nullptr;						// 디바이스	
	ID3D11DeviceContext* m_pDeviceContext = nullptr;		// 즉시 디바이스 컨텍스트
	IDXGISwapChain* m_pSwapChain = nullptr;					// 스왑체인
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;	// 렌더링 타겟뷰

	//viewport 정보 설정
	D3D11_VIEWPORT m_viewport = { 0 };
};

