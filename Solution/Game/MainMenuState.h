#pragma once
#include "GameState.h"

namespace Prism
{
	class Model2D;
	class Camera;
}

class MainMenuState : public GameState
{
public:
	MainMenuState();
	~MainMenuState();

	void InitState(CU::InputWrapper* anInputWrapper) override;
	void EndState() override;

	const eStateStatus Update() override;
	void Render() override;
	void ResumeState() override;

	void OnResize(int aWidth, int aHeight) override;

private:

	Prism::Model2D* myBackground;
	Prism::Camera* myCamera;
};

