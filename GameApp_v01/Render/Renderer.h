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
	void SetViewport();					// viewport ���� ����

public:
	static Renderer* m_pInstance;

private:
	DirectX::SimpleMath::Color color;	// �ʱ�ȭ ����

	// ������ ������������ �����ϴ� �ʼ� ��ü�� �������̽� ( �X�� ���ٽ� �䵵 ������ ���� ������� �ʴ´�.)
	ID3D11Device* m_pDevice = nullptr;						// ����̽�	
	ID3D11DeviceContext* m_pDeviceContext = nullptr;		// ��� ����̽� ���ؽ�Ʈ
	IDXGISwapChain* m_pSwapChain = nullptr;					// ����ü��
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;	// ������ Ÿ�ٺ�

	//viewport ���� ����
	D3D11_VIEWPORT m_viewport = { 0 };
};

