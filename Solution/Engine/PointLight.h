#pragma once
class PointLight
{
public:
	void Update();

	const CU::Vector4<float>& GetColor() const;
	void SetColor(const CU::Vector4<float>& aVector);

	const CU::Vector4<float>& GetPosition() const;
	void SetPosition(const CU::Vector4<float>& aPosition);

	void SetRange(float aRange);
	float GetRange() const;


	void PerformTransformation(const CU::Matrix44<float>& aTransformation);


private:
	CU::Vector4<float> myColor;
	CU::Vector4<float> myPosition;
	CU::Matrix44<float> myOrientation;
	CU::Vector4<float> myOriginalPosition;
	float myRange;
};

inline void PointLight::Update()
{
	myPosition = myOriginalPosition * myOrientation;
}

inline const CU::Vector4<float>& PointLight::GetColor() const
{
	return myColor;
}

inline void PointLight::SetColor(const CU::Vector4<float>& aColor)
{
	myColor = aColor;
}

inline const CU::Vector4<float>& PointLight::GetPosition() const
{
	return myPosition;
}

inline void PointLight::SetPosition(const CU::Vector4<float>& aPosition)
{
	myPosition = aPosition;
	myOriginalPosition = aPosition;
}

inline void PointLight::SetRange(float aRange)
{
	myRange = aRange;
}

inline float PointLight::GetRange() const
{
	return myRange;
}

inline void PointLight::PerformTransformation(const CU::Matrix44<float>& aTransformation)
{
	myOrientation = aTransformation * myOrientation;
}

