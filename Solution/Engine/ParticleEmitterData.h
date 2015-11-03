#pragma once
#include "ParticleData.h"

struct ID3D11InputLayout;

namespace Prism
{
	class ParticleEmitterInstance;
	class Texture;
	class Effect;


	class ParticleEmitterData
	{
	
		friend class ParticleEmitterInstance;
	
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

		float		 myMinRotation;
		float		 myMaxRotation;

		float		 myMaxSpeed;
		float		 myMinSpeed;
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