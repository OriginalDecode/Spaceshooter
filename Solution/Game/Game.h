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
};

class Entity;
class Player;

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

	Prism::Scene* myScene;
	Prism::Camera* myCamera;
	Prism::DirectionalLight* myLight;
	CU::Matrix44<float> myWorldMatrix;
	CU::GrowingArray<Entity*> myEntities;
	Player* myPlayer;

	bool myRenderStuff;
};