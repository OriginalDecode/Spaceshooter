#pragma once
#include "stdafx.h"
#include <Model.h>
#include <Camera.h>
#include <InputWrapper.h>

class Scene;
class Instance;
class Light;
class Text;
class Game
{
public:
	Game();
	~Game();

	bool Init(HWND& aHwnd);
	bool Destroy();
	bool Update();

	void Pause();
	void UnPause();
	void OnResize(int aWidth, int aHeight);

	

private:
	void operator=(Game& aApp) = delete;

	void LogicUpdate(const float aDeltaTime);
	void Render();

	CU::InputWrapper myInputWrapper;

	Model* myWaveModel;
	Model* myGravityModel;
	Model* myExtrudeModel;
	Model* myNormalModel;
	Model* myGeometryModel;
	Model* myPolygonModel;
	CU::GrowingArray<Instance*> myInstances;
	Scene* myScene;
	Camera myCamera;
	Light* myLight;
	CU::Matrix44<float> myWorldMatrix;

	bool myRenderStuff;
};

