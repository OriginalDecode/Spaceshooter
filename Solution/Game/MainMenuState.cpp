#include "stdafx.h"
#include "MainMenuState.h"
#include <Model2D.h>
#include <Camera.h>
#include <InputWrapper.h>

MainMenuState::MainMenuState(CU::InputWrapper* anInputWrapper)
{
	myInputWrapper = anInputWrapper;
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::InitState()
{
	myBackground = new Prism::Model2D;
	myBackground->Init("Data/resources/texture/seafloor.dds", { float(Prism::Engine::GetInstance()->GetWindowSize().x), 
		float(Prism::Engine::GetInstance()->GetWindowSize().y) });
	CU::Matrix44<float> orientation;
	myCamera = new Prism::Camera(orientation);
	OnResize(Prism::Engine::GetInstance()->GetWindowSize().x, Prism::Engine::GetInstance()->GetWindowSize().y);
}

void MainMenuState::EndState()
{
	delete myBackground;
	delete myCamera;
}

const eStateStatus MainMenuState::Update()
{
	if (myInputWrapper->KeyDown(DIK_ESCAPE))
	{
		return eStateStatus::ePopMainState;
	}

	Render();

	return eKeepState;
}

void MainMenuState::Render()
{
	myBackground->Render(*myCamera, 0.f, -100.f);
}

void MainMenuState::ResumeState()
{

}

void MainMenuState::OnResize(int aWidth, int aHeight)
{
	myCamera->OnResize(aWidth, aHeight);
}