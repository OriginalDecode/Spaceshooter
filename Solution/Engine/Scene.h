#pragma once
#include <GrowingArray.h>
#include <StaticArray.h>

namespace Prism{
	class Camera;
	class DirectionalLight;
	class Instance;
	class PointLight;

	class Scene
	{
	public:
		Scene();

		void Render();

		void AddInstance(Instance* aInstance);
		void AddLight(DirectionalLight* aLight);
		void AddLight(PointLight* aLight);

		void SetCamera(Camera* aCamera);
		Camera& GetCamera();

	private:
		CU::GrowingArray<Instance*> myInstances;
		CU::GrowingArray<DirectionalLight*> myDirectionalLights;
		CU::GrowingArray<PointLight*> myPointLights;

		Camera* myCamera;

		CU::StaticArray<CU::Vector4<float>, 1> myDirectionalLightDirections;
		CU::StaticArray<CU::Vector4<float>, 1> myDirectionalLightColors;

		CU::StaticArray<CU::Vector4<float>, 3> myPointLightPositions;
		CU::StaticArray<CU::Vector4<float>, 3> myPointLightColors;
		CU::StaticArray<float, 3> myPointLightRanges;
	};
}