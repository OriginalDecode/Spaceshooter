#include "stdafx.h"
#include "Scene.h"
#include "Instance.h"
#include "Light.h"

#include "DebugDataDisplay.h"


Scene::Scene()
{
	myInstances.Init(4);
	myLights.Init(4);
}


Scene::~Scene()
{
}

void Scene::Render()
{
	TIME_FUNCTION

	for (int i = 0; i < 1; ++i)
	{
		myDirections[i] = myLights[i]->GetCurrentLightDir();
		myColors[i] = myLights[i]->GetLightColor();
	}

	for (int i = 0; i < myInstances.Size(); ++i)
	{
		myInstances[i]->Render(*myCamera, myDirections, myColors);
	}
}

void Scene::AddInstance(Instance* aInstance)
{
	myInstances.Add(aInstance);
}

void Scene::AddLight(Light* aLight)
{
	myLights.Add(aLight);
}


void Scene::SetCamera(Camera* aCamera)
{
	myCamera = aCamera;
}

Camera& Scene::GetCamera()
{
	return *myCamera;
}