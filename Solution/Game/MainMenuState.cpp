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

void MainMenuState::InitState()
{
	myBackground = new Prism::Model2D;
	myBackground->Init("Data/resources/texture/seafloor.dds", { 640.f, 640.f });
	myCamera = new Prism::Camera(CU::Matrix44<float>());
}

void MainMenuState::EndState()
{
	delete myBackground;
	delete myCamera;
}

const eStateStatus& MainMenuState::Update()
{

}

void MainMenuState::Render()
{
	myBackground->Render(*myCamera, 0.f, 0.f);
}

void MainMenuState::ResumeState()
{

}