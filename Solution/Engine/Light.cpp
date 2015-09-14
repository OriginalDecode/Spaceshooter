#include "stdafx.h"
#include "Light.h"


Light::Light()
{
}


Light::~Light()
{
}

void Light::Update()
{
	CU::Vector3<float> newDir = myOriginalDirection * myOrientation;

	myDirection.x = newDir.x;
	myDirection.y = newDir.y;
	myDirection.z = newDir.z;
}
