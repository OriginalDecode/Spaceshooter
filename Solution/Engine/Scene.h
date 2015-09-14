#pragma once
#include <GrowingArray.h>
#include <StaticArray.h>
class Instance;
class Light;
class Camera;

class Scene
{
public:
	Scene();
	~Scene();

	void Render();

	void AddInstance(Instance* aInstance);
	void AddLight(Light* aLight);

	void SetCamera(Camera* aCamera);
	Camera& GetCamera();

private:
	CU::GrowingArray<Instance*> myInstances;
	CU::GrowingArray<Light*> myLights;
	Camera* myCamera;
	CU::StaticArray<CU::Vector4<float>, 1> myDirections;
	CU::StaticArray<CU::Vector4<float>, 1> myColors;
};

