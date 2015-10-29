#pragma once
#include <Matrix.h>
#include <GrowingArray.h>
#include <Vector.h>
#include <string>
#include <thread>
#include <Windows.h>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Prism
{
	struct TextCommand
	{
		std::string myText;
		CU::Vector2<float> myPosition;
		CU::Vector4<float> myColor;
		float myScale;
	};

	enum class eTextType
	{
		DEBUG_TEXT,
		RELEASE_TEXT,
	};


	class Camera;
	class DirectX;
	class EffectContainer;
	class Effect;
	class FBXFactory;
	class FileWatcher;
	class Font;
	class Model;
	class ModelLoader;
	class ModelProxy;
	class Text;
	class TextureContainer;

	struct SetupInfo;

	class Engine
	{
	public:
		static bool Create(HWND& aHwnd, WNDPROC aWndProc, SetupInfo& aSetupInfo);
		static void Destroy();
		static Engine* GetInstance();
		void Render();
		void OnResize(int aWidth, int aHeigth);

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetContex();
		TextureContainer* GetTextureContainer();
		EffectContainer* GetEffectContainer();
		FileWatcher* GetFileWatcher();
		ModelLoader* GetModelLoader();

		Model* DLLLoadModel(const std::string& aModelPath, Effect* aEffect);

		const CU::Vector2<int>& GetWindowSize() const;
		const CU::Matrix44<float>& GetOrthogonalMatrix() const;

		void PrintText(const std::string& aText, const CU::Vector2<float>& aPosition, eTextType aTextType, float aScale = 1.f);
		void PrintText(float aNumber, const CU::Vector2<float>& aPosition, eTextType aTextType, float aScale = 1.f);
		void PrintText(int aNumber, const CU::Vector2<float>& aPosition, eTextType aTextType, float aScale = 1.f);

		void EnableZBuffer();
		void DisableZBuffer();

		void ToggleWireframe();

		void EnableWireframe();
		void DisableWireframe();


		void SetClearColor(const CU::Vector4<float>& aClearColor);

		bool myWireframeShouldShow;

	private:
		Engine();
		~Engine();
		bool Init(HWND& aHwnd, WNDPROC aWndProc);
		bool WindowSetup(HWND& aHwnd, WNDPROC aWindowProc);
		
		bool myWireframeIsOn;

		DirectX* myDirectX;
		SetupInfo* mySetupInfo;
		TextureContainer* myTextureContainer;
		EffectContainer* myEffectContainer;
		FBXFactory* myModelFactory;
		FileWatcher* myFileWatcher;

		Font* myFont;
		Text* myText;

		CU::Vector4<float> myClearColor;
		CU::Vector2<int> myWindowSize;
		CU::Matrix44<float> myOrthogonalMatrix;


		ModelLoader* myModelLoader;
		std::thread* myModelLoaderThread;

		static Engine* myInstance;

		CU::GrowingArray<TextCommand> myTexts;
		CU::GrowingArray<TextCommand> myDebugTexts;
	};
}

inline Prism::TextureContainer* Prism::Engine::GetTextureContainer()
{
	return myTextureContainer;
}

inline Prism::EffectContainer* Prism::Engine::GetEffectContainer()
{
	return myEffectContainer;
}

inline Prism::FileWatcher* Prism::Engine::GetFileWatcher()
{
	return myFileWatcher;
}

inline Prism::ModelLoader* Prism::Engine::GetModelLoader()
{
	return myModelLoader;
}

inline const CU::Vector2<int>& Prism::Engine::GetWindowSize() const
{
	return myWindowSize;
}

inline const CU::Matrix44<float>& Prism::Engine::GetOrthogonalMatrix() const
{
	return myOrthogonalMatrix;
}

inline void Prism::Engine::SetClearColor(const CU::Vector4<float>& aClearColor)
{
	myClearColor = aClearColor;
}