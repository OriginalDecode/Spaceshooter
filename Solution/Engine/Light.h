#pragma once

#include <Matrix.h>

class Light
{
public:
	Light();
	~Light();

	void Update();

	CU::Vector4<float> GetCurrentLightDir() const;
	const CU::Vector4<float>& GetLightColor() const;
	const CU::Vector3<float>& GetLightDir() const;

	void PerformRotation(const CU::Matrix33<float>& aOrientation);

	void SetLightColor(const CU::Vector4<float>& aVector);
	void SetLightDir(const CU::Vector3<float>& aVector);

private:
	CU::Vector4<float> myColor;
	CU::Vector4<float> myDirection;
	CU::Vector3<float> myOriginalDirection;
	CU::Matrix33<float> myOrientation;
};

inline CU::Vector4<float> Light::GetCurrentLightDir() const
{
	return myDirection;
}

inline const CU::Vector4<float>& Light::GetLightColor() const
{
	return myColor;
}

inline const CU::Vector3<float>& Light::GetLightDir() const
{
	return myOriginalDirection;
}

 
inline void Light::PerformRotation(const CU::Matrix33<float>& aOrientation)
{
	myOrientation = aOrientation * myOrientation;
}

 
inline void Light::SetLightColor(const CU::Vector4<float>& aVector)
{
	myColor = aVector;
}

inline void Light::SetLightDir(const CU::Vector3<float>& aVector)
{
	myOriginalDirection = aVector;
	myDirection = aVector;
}