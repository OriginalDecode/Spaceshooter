#include "stdafx.h"
#include "Instance.h"
#include "Effect.h"
#include "Light.h"


Instance::Instance(Model& aModel)
	: myModel(aModel)
{
}


Instance::~Instance()
{
}

bool Instance::Init()
{
	return true;
}

void Instance::Render(Camera& aCamera, CU::StaticArray<CU::Vector4<float>, 1> someLightDirections, CU::StaticArray<CU::Vector4<float>, 1> someLightColors)
{
	myModel.GetEffect()->UpdateLight(someLightDirections, someLightColors);

	myModel.GetEffect()->SetViewMatrix(CU::InverseSimple(aCamera.GetOrientation()));
	myModel.GetEffect()->SetProjectionMatrix(aCamera.GetProjection());

	myModel.Render(myOrientation);
}

void Instance::Render(const CU::Matrix44<float>& aParentMatrix, Camera& aCamera, CU::StaticArray<CU::Vector4<float>, 1> someLightDirections, CU::StaticArray<CU::Vector4<float>, 1> someLightColors)
{
	myModel.GetEffect()->UpdateLight(someLightDirections, someLightColors);

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