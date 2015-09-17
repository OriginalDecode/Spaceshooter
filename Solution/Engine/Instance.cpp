#include "stdafx.h"

#include "Camera.h"
#include "Effect.h"
#include "Instance.h"
#include "Model.h"


Instance::Instance(Model& aModel)
	: myModel(aModel)
{
}

void Instance::Render(Camera& aCamera)
{
	myModel.GetEffect()->SetViewMatrix(CU::InverseSimple(aCamera.GetOrientation()));
	myModel.GetEffect()->SetProjectionMatrix(aCamera.GetProjection());

	myModel.Render(myOrientation);
}

void Instance::Render(const CU::Matrix44<float>& aParentMatrix, Camera& aCamera)
{
	myModel.GetEffect()->SetViewMatrix(CU::InverseSimple(aCamera.GetOrientation()));
	myModel.GetEffect()->SetProjectionMatrix(aCamera.GetProjection());

	myModel.Render(myOrientation * aParentMatrix);
}

void Instance::SetPosition(const CU::Vector3<float>& aPosition)
{
	myPosition = aPosition;
	myOrientation.SetPos(aPosition);
}

CU::Vector3<float>& Instance::GetPosition()
{
	return myPosition;
}

CU::Matrix44<float>& Instance::GetOrientation()
{
	return myOrientation;
}

void Instance::SetOrientation(const CU::Matrix44<float>& aOrientation)
{
	myOrientation = aOrientation;
}

void Instance::PerformRotationLocal(CU::Matrix44<float>& aRotation)
{
	CU::Vector3<float> oldPos = myOrientation.GetPos();
	myOrientation.SetPos({ 0.f, 0.f, 0.f, 1.f });
	myOrientation = myOrientation * aRotation;
	myOrientation.SetPos(oldPos);
}

void Instance::PerformRotationWorld(CU::Matrix44<float>& aRotation)
{
	myOrientation = myOrientation * aRotation;
}

void Instance::PerformTransformation(CU::Matrix44<float>& aTransformation)
{
	myOrientation = myOrientation * aTransformation;
}

void Instance::UpdateDirectionalLights(const CU::StaticArray<CU::Vector4<float>, 1>& someLightDirections, 
		const CU::StaticArray<CU::Vector4<float>, 1>& someLightColors)
{
	myModel.GetEffect()->UpdateDirectionalLight(someLightDirections, someLightColors);
}

void Instance::UpdatePointLights(const CU::StaticArray<CU::Vector4<float>, 3>& someLightPositions, 
		const CU::StaticArray<CU::Vector4<float>, 3>& someLightColors, const CU::StaticArray<float, 3>& someLightRanges)
{
	myModel.GetEffect()->UpdatePointLight(someLightPositions, someLightColors, someLightRanges);
}