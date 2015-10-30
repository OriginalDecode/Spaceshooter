#pragma once
#include "ParticleData.h"

struct ID3D11InputLayout;

namespace Prism
{
	class EmitterInstance;
	class Texture;
	class Effect;


	class EmitterData
	{
	
		friend class EmitterInstance;
	
	public:

		void LoadDataFile(const char* aFilePath);

	private:

		void CreateInputLayout();
		
		Texture* myTexture;
		Effect* myEffect;
		std::string myFileName;
		ParticleData myData;

		ID3D11InputLayout* myInputLayout;

		CU::Vector3f myEmitterSize;
		CU::Vector3f myEmissionVelocityDelta;

		CU::Vector3f myMaxRotation;
		CU::Vector3f myMinRotation;
		CU::Vector3f myRotationDelta;

		CU::Vector3f myMaxSpeed;
		CU::Vector3f myMinSpeed;
		CU::Vector3f myDirection;

		std::string	myEffectName;
		std::string	myTextureName;

		float myParticlesLifeTime;
		float myEmitterLifeTime;

		float myEmissionLifeTime;
		float myEmissionRate;
		float myEmissionRateDelta;

		float myMinScale;
		float myMaxScale;
		int myParticlesPerEmitt;
		int	myMaxParticleAmount;

		bool myIsActiveAtStart;
		bool myUseEmitterLifeTime;


	};
}