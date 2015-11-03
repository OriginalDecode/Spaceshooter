#pragma once
#include <Vector.h>

struct ParticleData
{

	float myMaxSpeed;
	float myMinSpeed;

	CU::Vector3f myStartColor;
	CU::Vector3f myEndColor;

	float myLifeTime;
	
	float myMaxStartSize;
	float myMinStartSize;

	float mySizeDelta;

	float myStartAlpha;
	float myAlphaDelta;

};

struct GraphicalParticle
{
	GraphicalParticle()
		: myAlpha(0.0f)
		, mySize(0.0f)
		, myLifeTime(10.0f)
		, myRotation(0.f)
	{
	}

	CU::Vector3f myPosition;
	CU::Vector3f myColor;
	float myAlpha;
	float mySize;
	float myLifeTime;
	float myRotation;
};

struct LogicalParticle
{
	LogicalParticle()
		: mySpeed(0)
		, myIsAlive(false)
		, myRotation(0)
	{
	}
	bool myIsAlive;
	float mySpeed;
	CU::Vector3f myDirection;
	float myRotation;

};
