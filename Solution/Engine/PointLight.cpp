#include "stdafx.h"

#include "Camera.h"
#include "Instance.h"
#include "Model.h"
#include "PointLight.h"

namespace Prism
{

	void PointLight::Initiate()
	{
		Model* model;
		model = new Model();
		model->InitCube(1, 1, 1);
		myInstance = new Instance(*model);
	}

	void PointLight::Render(Camera* aCamera)
	{
		myInstance->Render(*aCamera);
	}

};