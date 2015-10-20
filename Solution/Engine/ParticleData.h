#pragma once

struct ParticleData
{

	CU::Vector3f myMaxVelocity;
	CU::Vector3f myMinVelocity;

	float myLifeTime;
	
	float myMaxStartSize;
	float myMinStartSize;

	float mySizeDelta;

	float myStartAlpha;
	float myAlphaDelta;

};

struct ParticleInstance
{
	ParticleInstance()
	{
		myAlpha = 1.0f;
		mySize = 1.0f;
		myLifeTime = 10.0f;
		myIsAlive = false;
	}
	CU::Vector3f	myPosition;
	CU::Vector3f	myVelocity;

	float			myAlpha;
	float			mySize;
	float			myLifeTime;
	bool			myIsAlive;

};