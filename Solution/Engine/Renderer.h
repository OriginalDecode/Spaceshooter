#pragma once

#include "EngineEnums.h"

#define MAX_NUMBER_OF_SCENES 2
namespace Prism
{
	class FullScreenHelper;
	class Texture;

	struct SceneData;

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void BeginScene();
		void EndScene(int aEffect);

		void FinalRender();

		void OnResize(float aWidth, float aHeight);

	private:
		struct SceneData
		{
			Texture* myScene;
			Texture* myFinished;
		};
		
		Engine* myEngine;
		SceneData mySceneData[MAX_NUMBER_OF_SCENES];

		FullScreenHelper* myFullScreenHelper;
		float myClearColor[4];
		int mySceneIndex;

		Texture* myFinalTexture;
		Texture* myCombineMiddleMan;
	};
}

