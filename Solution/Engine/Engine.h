#pragma once

#include "EffectContainer.h"
#include "FBXFactory.h"
#include "FontContainer.h"
#include "DebugDataDisplay.h"

class Text;

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Prism
{
	class DirectX;
	class TextureContainer;	

	struct SetupInfo;

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
		TextureContainer* GetTextureContainer();
		EffectContainer& GetEffectContainer();
		FontContainer& GetFontContainer();
		DebugDataDisplay& GetDebugDisplay();
		Model* LoadModel(const std::string& aPath, Effect* aEffect);

		void PrintDebugText(const Camera& aCamera, const std::string& aText
			, const CU::Vector2<float>& aPosition, float aScale = 1.f);

		void EnableZBuffer();
		void DisableZBuffer();
	private:
		Engine();
		~Engine();
		bool Init(HWND& aHwnd, WNDPROC aWndProc);
		bool WindowSetup(HWND& aHwnd, WNDPROC aWindowProc);


		DirectX* myDirectX;
		SetupInfo* mySetupInfo;
		TextureContainer* myTextureContainer;
		EffectContainer myEffectContainer;
		FBXFactory myModelFactory;
		FontContainer myFontContainer;
		DebugDataDisplay myDebugDataDisplay;

		Text* myDebugText;

		static Engine* myInstance;
	};
}

inline Prism::TextureContainer* Prism::Engine::GetTextureContainer()
{
	return myTextureContainer;
}

inline Prism::EffectContainer& Prism::Engine::GetEffectContainer()
{
	return myEffectContainer;
}

inline Prism::FontContainer& Prism::Engine::GetFontContainer()
{
	return myFontContainer;
}

inline Prism::DebugDataDisplay& Prism::Engine::GetDebugDisplay()
{
	return myDebugDataDisplay;
}