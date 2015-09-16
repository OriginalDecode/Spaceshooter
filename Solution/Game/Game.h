#pragma once
#include <Matrix.h>

namespace CommonUtilities
{
	class InputWrapper;
}

class Camera;
class DirectionalLight;
class Model;
class PointLight;
class Scene;
class Text;
class Instance;

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

	CU::InputWrapper* myInputWrapper;

	Model* myWaveModel;
	Model* myGravityModel;
	Model* myExtrudeModel;
	Model* myNormalModel;
	Model* myGeometryModel;
	Model* myPolygonModel;
	CU::GrowingArray<Instance*> myInstances;
	Scene* myScene;
	Camera* myCamera;
	DirectionalLight* myLight;
	PointLight* myPointLight;
	CU::Matrix44<float> myWorldMatrix;

	bool myRenderStuff;
};

