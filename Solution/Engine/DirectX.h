#pragma once
#pragma warning( disable : 4005 )
#include <d3d11.h>
#include "SetupInfo.h"

class DirectX
{
public:
	DirectX(HWND& aHwnd, SetupInfo& aSetupInfo);
	~DirectX();

	void Present(const unsigned int aSyncInterval, const unsigned int aFlags);
	void Clear(const float aClearColor[4]);

	void OnResize(int aWidth, int aHeigth);
	void CleanD3D();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContex();

	void EnableZBuffer();
	void DisableZBuffer();

private:
	void operator=(const DirectX&) = delete;

	bool D3DSetup();
	bool D3DSwapChainSetup();
	bool D3DRenderTargetSetup();
	bool D3DViewPortSetup(int aWidth, int aHeight);
	bool D3DStencilBufferSetup(int aWidth, int aHeight);
	bool D3DEnabledStencilStateSetup();
	bool D3DDisabledStencilStateSetup();

	ID3D11Device* myDevice;
	ID3D11DeviceContext* myContext;
	IDXGISwapChain* mySwapChain;
	ID3D11RenderTargetView* myRenderTargetView;
	ID3D11DepthStencilView* myDepthBufferView;
	ID3D11DepthStencilState* myEnabledDepthStencilState;
	ID3D11DepthStencilState* myDisabledDepthStencilState;
	ID3D11Texture2D* myDepthBuffer;

	HWND& myHWND;
	SetupInfo mySetupInfo;
};

inline ID3D11Device* DirectX::GetDevice()
{
	return myDevice;
}

inline ID3D11DeviceContext* DirectX::GetContex()
{
	return myContext;
}