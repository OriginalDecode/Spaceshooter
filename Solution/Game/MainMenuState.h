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

	void InitState() override;
	void EndState() override;

	const eStateStatus& Update() override;
	void Render() override;
	void ResumeState() override;

private:

	Prism::Model2D* myBackground;
	Prism::Camera* myCamera;
};

