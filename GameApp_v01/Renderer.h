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
	D3D11_VIEWPORT m_viewport = { 0 };						//viewport ���� ����

	// ������ ���������ο� �����ϴ� ��ü�� ����
	ID3D11VertexShader* m_pVertexShader = nullptr;	// ���� ���̴�.
	ID3D11PixelShader* m_pPixelShader = nullptr;	// �ȼ� ���̴�.	
	ID3D11InputLayout* m_pInputLayout = nullptr;	// �Է� ���̾ƿ�.
	ID3D11Buffer* m_pVertexBuffer = nullptr;		// ���ؽ� ����.
	UINT m_VertextBufferStride = 0;					// ���ؽ� �ϳ��� ũ��.
	UINT m_VertextBufferOffset = 0;					// ���ؽ� ������ ������.
	UINT m_VertexCount = 0;							// ���ؽ� ����.


	// �ӽ� (���߿� Scene���� �и� �ؾ���)
public:
	bool InitScene();		// ���̴�,���ؽ�,�ε���
	void RenderScene();
	void UnInitScene();

};

