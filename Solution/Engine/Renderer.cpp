#include "stdafx.h"

#include "FullScreenHelper.h"
#include "Texture.h"
#include "Renderer.h"

namespace Prism
{
	Renderer::Renderer()
	{
		CU::Vector2<float> screenSize(static_cast<float>(Engine::GetInstance()->GetWindowSize().x)
			, static_cast<float>(Engine::GetInstance()->GetWindowSize().y));

		for (int i = 0; i < MAX_NUMBER_OF_SCENES; ++i)
		{
			mySceneData[i].myScene = new Texture();
			mySceneData[i].myScene->Init(screenSize.x, screenSize.y, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
				, DXGI_FORMAT_R32G32B32A32_FLOAT);

			mySceneData[i].myFinished = new Texture();
			mySceneData[i].myFinished->Init(screenSize.x, screenSize.y, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
				, DXGI_FORMAT_R32G32B32A32_FLOAT);
		}

		myFinalTexture = new Texture();
		myFinalTexture->Init(screenSize.x, screenSize.y, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R32G32B32A32_FLOAT);

		myCombineMiddleMan = new Texture();
		myCombineMiddleMan->Init(screenSize.x, screenSize.y, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R32G32B32A32_FLOAT);

		myFullScreenHelper = new FullScreenHelper();

		myClearColor[0] = 0.f;
		myClearColor[1] = 0.f;
		myClearColor[2] = 0.f;
		myClearColor[3] = 0.f;

		mySceneIndex = 0;
	}
	
	
	Renderer::~Renderer()
	{
		for (int i = 0; i < MAX_NUMBER_OF_SCENES; ++i)
		{
			delete mySceneData[i].myScene;
			mySceneData[i].myScene = nullptr;

			delete mySceneData[i].myFinished;
			mySceneData[i].myFinished = nullptr;
		}

		delete myFullScreenHelper;
		myFullScreenHelper = nullptr;
	}

	void Renderer::BeginScene()
	{
		DL_ASSERT_EXP(mySceneIndex < MAX_NUMBER_OF_SCENES, "Tried to Begin to many Scenes");

		SceneData& data = mySceneData[mySceneIndex];

		ID3D11RenderTargetView* renderTarget = data.myScene->GetRenderTargetView();
		Engine::GetInstance()->GetContex()->ClearRenderTargetView(renderTarget, myClearColor);
		Engine::GetInstance()->GetContex()->OMSetRenderTargets(1, &renderTarget
			, Engine::GetInstance()->GetDepthView());
	}

	void Renderer::EndScene(int aEffect)
	{
		DL_ASSERT_EXP(mySceneIndex < MAX_NUMBER_OF_SCENES, "Tried to Begin to many Scenes");

		SceneData& data = mySceneData[mySceneIndex];

		ID3D11RenderTargetView* renderTarget = data.myFinished->GetRenderTargetView();
		Engine::GetInstance()->GetContex()->ClearRenderTargetView(renderTarget, myClearColor);

		myFullScreenHelper->Process(data.myScene, data.myFinished, aEffect);

		++mySceneIndex;
	}

	void Renderer::FinalRender()
	{
		//Engine::GetInstance()->GetContex()->ClearRenderTargetView(myFinalTexture->GetRenderTargetView(), myClearColor);
		//Engine::GetInstance()->GetContex()->ClearRenderTargetView(myCombineMiddleMan->GetRenderTargetView(), myClearColor);
		//
		//Engine::GetInstance()->DisableZBuffer();
		//
		//if (mySceneIndex > 1)
		//{
		//	myFullScreenHelper->CombineTextures(mySceneData[0].myFinished, mySceneData[1].myFinished, myCombineMiddleMan);
		//	myFullScreenHelper->CopyTexture(myCombineMiddleMan, myFinalTexture);
		//
		//	for (int i = 2; i < mySceneIndex; ++i)
		//	{
		//		myFullScreenHelper->CombineTextures(mySceneData[i].myFinished, myFinalTexture, myCombineMiddleMan);
		//		myFullScreenHelper->CopyTexture(myCombineMiddleMan, myFinalTexture);
		//	}
		//}
		//else
		//{
		//	myFullScreenHelper->CopyTexture(mySceneData[0].myFinished, myFinalTexture);
		//}
		//
		//
		//myFullScreenHelper->RenderToScreen(myFinalTexture);

		for (int i = 0; i < mySceneIndex; ++i)
		{
			Engine::GetInstance()->DisableZBuffer();
			myFullScreenHelper->RenderToScreen(mySceneData[i].myFinished);
		}
		Engine::GetInstance()->EnableZBuffer();

		mySceneIndex = 0;
	}

}
