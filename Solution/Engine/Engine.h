#pragma once
#include "TextureContainer.h"
#include "EffectContainer.h"
#include "FBXFactory.h"
#include "FontContainer.h"
#include "DebugDataDisplay.h"

class DirectX;
struct SetupInfo;
struct ID3D11Device;
struct ID3D11DeviceContext;

class Engine
{
public:
	static bool Create(HWND& aHwnd, WNDPROC aWndProc, SetupInfo& aSetupInfo);
	static Engine* GetInstance();
	void Shutdown();
	void Render();
	void OnResize(int aWidth, int aHeigth);

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContex();
	TextureContainer& GetTextureContainer();
	EffectContainer& GetEffectContainer();
	FontContainer& GetFontContainer();
	DebugDataDisplay& GetDebugDisplay();
	Model* LoadModel(const std::string& aPath, Effect* aEffect);

	void EnableZBuffer();
	void DisableZBuffer();
private:
	bool Init(HWND& aHwnd, WNDPROC aWndProc);
	bool WindowSetup(HWND& aHwnd, WNDPROC aWindowProc);
	

	DirectX* myDirectX;
	SetupInfo* mySetupInfo;
	TextureContainer myTextureContainer;
	EffectContainer myEffectContainer;
	FBXFactory myModelFactory;
	FontContainer myFontContainer;
	DebugDataDisplay myDebugDataDisplay;

	static Engine* myInstance;
};


inline TextureContainer& Engine::GetTextureContainer()
{
	return myTextureContainer;
}

inline EffectContainer& Engine::GetEffectContainer()
{
	return myEffectContainer;
}

inline FontContainer& Engine::GetFontContainer()
{
	return myFontContainer;
}

inline DebugDataDisplay& Engine::GetDebugDisplay()
{
	return myDebugDataDisplay;
}