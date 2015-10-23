#include "stdafx.h"

#include "Camera.h"
#include "EngineEnums.h"
#include "Instance.h"
#include "ModelLoader.h"
#include "Model.h"
#include "PointLight.h"

namespace Prism
{

	void PointLight::Initiate()
	{
		myObjectCullingRadius = 10.f;
		ModelProxy* model = Engine::GetInstance()->GetModelLoader()->LoadCube(1, 1, 1);
		myInstance = new Instance(*model, myOrientation, eOctreeType::NOT_IN_OCTREE, myObjectCullingRadius);
	}

	void PointLight::Render(Camera* aCamera)
	{
		if (this != nullptr)
			myInstance->Render(*aCamera);
	}

};