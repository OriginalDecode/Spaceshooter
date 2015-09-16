#include "stdafx.h"
#include "PointLight.h"


PointLight::PointLight()
{
}


PointLight::~PointLight()
{
}

void PointLight::Update()
{
	myPosition = myOriginalPosition * myOrientation;
}
