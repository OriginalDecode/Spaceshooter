#pragma once

#include "ParticleEmitterData.h"
#include "ParticleData.h"

namespace Prism
{
	class Camera;
	struct VertexBufferWrapper;
	
	class ParticleEmitterInstance
	{
	public:

		ParticleEmitterInstance();
		~ParticleEmitterInstance();
		void Initiate(ParticleEmitterData* someData);
		void Render(Camera* aCamera);
		void Update(float aDeltaTime, const CU::Matrix44f& aWorldMatrix);
		bool GetIsActive();
		void ToggleActive();
		void ToggleActive(bool aIsActive);
		void ShouldLive(bool aToStop);
		void SetPosition(CU::Vector3f aPosition);
	private:

		void CreateVertexBuffer();
		void UpdateVertexBuffer();

		void UpdateEmitter(float aDeltaTime, const CU::Matrix44f& aWorldMatrix);
		void UpdateParticle(float aDeltaTime);

		void EmitParticle(const CU::Matrix44f& aWorldMatrix);

		CU::GrowingArray<LogicalParticle> myLogicalParticles;
		CU::GrowingArray<GraphicalParticle> myGraphicalParticles;

		CU::Vector3f myDiffColor;
		CU::Matrix44f myOrientation;

		ParticleEmitterData* myParticleEmitterData;
		VertexBufferWrapper *myVertexWrapper;

		float myEmissionTime;
		float myEmitterLife;
		float myParticleScaling;

		int myParticleIndex;
		int myDeadParticleCount;

		bool myIsActive;
		bool myShouldLive;
	};

	inline void ParticleEmitterInstance::ShouldLive(bool aToStop)
	{
		myShouldLive = aToStop;
	}


}
