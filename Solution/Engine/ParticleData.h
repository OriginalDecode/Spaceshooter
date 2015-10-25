#pragma once
#include <Vector.h>

struct ParticleData
{

	CU::Vector3f myMaxVelocity;
	CU::Vector3f myMinVelocity;

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
	{
	}

	CU::Vector3f myPosition;
	CU::Vector3f myColor;
	float myAlpha;
	float mySize;
	float myLifeTime;
};

struct LogicalParticle
{
	LogicalParticle()
		: myVelocity(CU::Vector3f(0, 0, 0))
	{
	}
	CU::Vector3f myVelocity;
};
