#pragma once

#include "EmitterData.h"
#include "ParticleData.h"

namespace Prism
{
	class Camera;
	struct VertexBufferWrapper;
	
	class EmitterInstance
	{
	public:
		~EmitterInstance();
		void Initiate(EmitterData someData);
		void Render(Camera* aCamera);
		void Update(float aDeltaTime, const CU::Matrix44f& aWorldMatrix);
		bool GetIsActive();
		void ToggleActive();
	private:

		void CreateVertexBuffer();
		void UpdateVertexBuffer();

		void UpdateEmitter(float aDeltaTime, const CU::Matrix44f& aWorldMatrix);
		void UpdateParticle(float aDeltaTime);

		void EmittParticle(const CU::Matrix44f& aWorldMatrix);

		CU::GrowingArray<LogicalParticle> myLogicalParticles;
		CU::GrowingArray<GraphicalParticle> myGraphicalParticles;

		CU::Vector3f myDiffColor;
		CU::Matrix44f myOrientation;

		EmitterData myEmitterData;
		VertexBufferWrapper *myVertexWrapper;

		float myEmissionTime;
		float myEmissionRate;
		int myParticleIndex;
		float myParticleScaling;

		bool myIsActive;
	};
}
