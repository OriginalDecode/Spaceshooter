#include "stdafx.h"
#include "MainMenuState.h"
#include <Model2D.h>
#include <Camera.h>

MainMenuState::MainMenuState()
{
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::InitState(CU::InputWrapper* anInputWrapper)
{
	myInputWrapper = anInputWrapper;
	myBackground = new Prism::Model2D;
	myBackground->Init("Data/resources/texture/seafloor.dds", { 640.f, 640.f });
	CU::Matrix44<float> orientation;
	myCamera = new Prism::Camera(orientation);
}

void MainMenuState::EndState()
{
	delete myBackground;
	delete myCamera;
}

const eStateStatus MainMenuState::Update()
{
	return myStateStatus;
}

void MainMenuState::Render()
{
	myBackground->Render(*myCamera, 0.f, 0.f);
}

void MainMenuState::ResumeState()
{

}

void MainMenuState::OnResize(int aWidth, int aHeight)
{

}