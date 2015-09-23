#pragma once
#include <Matrix.h>
#include <GrowingArray.h>
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
};

class Entity;
class BulletManager;

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

	Prism::Model* mySkyboxModel;
	Prism::Instance* mySkybox;

	Prism::Scene* myScene;
	Prism::Camera* myCamera;
	Prism::DirectionalLight* myLight;
	Prism::PointLight* myPointLight;
	CU::Matrix44<float> myWorldMatrix;
	CU::GrowingArray<Entity*> myEntities;
	Entity* myPlayer;
	BulletManager* myBulletManager;

	bool myRenderStuff;
	bool myShowPointLightCube;
};