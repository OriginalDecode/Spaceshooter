#pragma once
#include <Matrix.h>

namespace CommonUtilities
{
	class InputWrapper;
}

namespace Prism
{
	class Camera;
	class DirectionalLight;
	class Model;
	class PointLight;
	class Scene;
	class Text;
	class Instance;
}

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

	Prism::Model* myWaveModel;
	Prism::Model* myGravityModel;
	Prism::Model* myExtrudeModel;
	Prism::Model* myNormalModel;
	Prism::Model* myGeometryModel;
	Prism::Model* myPolygonModel;
	CU::GrowingArray<Prism::Instance*> myInstances;
	Prism::Scene* myScene;
	Prism::Camera* myCamera;
	Prism::DirectionalLight* myLight;
	Prism::PointLight* myPointLight;
	CU::Matrix44<float> myWorldMatrix;

	bool myRenderStuff;
};

