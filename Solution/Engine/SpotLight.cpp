#include "stdafx.h"

#include "EngineEnums.h"
#include "Model.h"
#include "ModelLoader.h"
#include "Instance.h"
#include "SpotLight.h"


namespace Prism
{

	void SpotLight::Initiate()
	{
		myObjectCullingRadius = 10.f;
		ModelProxy* model = Engine::GetInstance()->GetModelLoader()->LoadCube(1, 1, 1);
		myInstance = new Instance(*model, myOrientation, eOctreeType::NOT_IN_OCTREE, myObjectCullingRadius);
	}

	void SpotLight::Render(Camera* aCamera)
	{
		if (myInstance != nullptr)
		{
			myInstance->Render(*aCamera);
		}
	}

};