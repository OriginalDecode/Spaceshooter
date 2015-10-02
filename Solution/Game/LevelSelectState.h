#pragma once
#include "GameState.h"

namespace Prism
{
	class Model2D;
	class Camera;
}

class LevelSelectState : public GameState
{
public:
	LevelSelectState(CU::InputWrapper* anInputWrapper);
	~LevelSelectState();

	void InitState(StateStackProxy* aStateStackProxy) override;
	void EndState() override;

	const eStateStatus Update() override;
	void Render() override;
	void ResumeState() override;

	void OnResize(int aWidth, int aHeight) override;

private:

	Prism::Model2D* myBackground;
	Prism::Camera* myCamera;
};

