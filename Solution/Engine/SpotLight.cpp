#include "stdafx.h"

#include "Model.h"
#include "Instance.h"
#include "SpotLight.h"


namespace Prism
{

	void SpotLight::Initiate()
	{
		Model* model;
		model = new Model();
		model->InitLightCube(1, 1, 1);
		myInstance = new Instance(*model);
	}

	void SpotLight::Render(Camera* aCamera)
	{
		myInstance->Render(*aCamera);
	}

};