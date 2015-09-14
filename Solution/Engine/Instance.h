#pragma once

#include "Camera.h"
#include "Model.h"
#include <Matrix.h>
#include <StaticArray.h>


class Light;

class Instance
{
public:
	Instance(Model& aModel);
	~Instance();

	bool Init();
	void Render(Camera& aCamera, CU::StaticArray<CU::Vector4<float>, 1> someLightDirections, CU::StaticArray<CU::Vector4<float>, 1> someLightColors);
	void Render(const CU::Matrix44<float>& aParentMatrix, Camera& aCamera, CU::StaticArray<CU::Vector4<float>, 1> someLightDirections, CU::StaticArray<CU::Vector4<float>, 1> someLightColors);

	void SetPosition(const CU::Vector3<float>& aPosition);
	CU::Vector3<float>& GetPosition();
	CU::Matrix44<float>& GetOrientation();
	void SetOrientation(const CU::Matrix44<float>& aOrientation);
	void PerformRotationLocal(CU::Matrix44<float>& aRotation);
	void PerformRotationWorld(CU::Matrix44<float>& aRotation);
	void PerformTransformation(CU::Matrix44<float>& aTransformation);


private:
	void operator=(Instance&) = delete;

	Model& myModel;
	CU::Matrix44<float> myOrientation;
	CU::Vector3<float> myPosition;
};

