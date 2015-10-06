#pragma once
#include "GameState.h"

namespace Prism
{
	class Model2D;
	class Camera;
}

class Menu;

class MenuState : public GameState
{
public:
	MenuState(const std::string& aXMLPath, CU::InputWrapper* anInputWrapper);
	~MenuState();

	void InitState(StateStackProxy* aStateStackProxy) override;
	void EndState() override;

	const eStateStatus Update() override;
	void Render() override;
	void ResumeState() override;

	void OnResize(int aWidth, int aHeight) override;

protected:

	Menu* myMenu;
	Prism::Camera* myCamera;
};

