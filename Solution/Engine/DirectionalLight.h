#pragma once

#include <Matrix.h>

class DirectionalLight
{
public:
	DirectionalLight();
	~DirectionalLight();

	void Update();

	CU::Vector4<float> GetCurrentDir() const;
	const CU::Vector4<float>& GetColor() const;
	const CU::Vector3<float>& GetDir() const;

	void PerformRotation(const CU::Matrix33<float>& aOrientation);

	void SetColor(const CU::Vector4<float>& aVector);
	void SetDir(const CU::Vector3<float>& aVector);

private:
	CU::Vector4<float> myColor;
	CU::Vector4<float> myDirection;
	CU::Vector3<float> myOriginalDirection;
	CU::Matrix33<float> myOrientation;
};

inline CU::Vector4<float> DirectionalLight::GetCurrentDir() const
{
	return myDirection;
}

inline const CU::Vector4<float>& DirectionalLight::GetColor() const
{
	return myColor;
}

inline const CU::Vector3<float>& DirectionalLight::GetDir() const
{
	return myOriginalDirection;
}

 
inline void DirectionalLight::PerformRotation(const CU::Matrix33<float>& aOrientation)
{
	myOrientation = aOrientation * myOrientation;
}

 
inline void DirectionalLight::SetColor(const CU::Vector4<float>& aVector)
{
	myColor = aVector;
}

inline void DirectionalLight::SetDir(const CU::Vector3<float>& aVector)
{
	myOriginalDirection = aVector;
	myDirection = aVector;
}