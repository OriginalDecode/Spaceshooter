#include "stdafx.h"
#include "DirectionalLight.h"


DirectionalLight::DirectionalLight()
{
}


DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::Update()
{
	CU::Vector3<float> newDir = myOriginalDirection * myOrientation;

	myDirection.x = newDir.x;
	myDirection.y = newDir.y;
	myDirection.z = newDir.z;
}
