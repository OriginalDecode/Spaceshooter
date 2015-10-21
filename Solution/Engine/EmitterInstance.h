#pragma once

#include "EmitterData.h"
#include "ParticleData.h"

//struct ParticleStruct
//{
//	float myAlphaDelta;
//	float mySizeDelta;
//	float myStartAlpha;
//	float dummy; //Memory Alignment
//};

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
		void Update(float aDeltaTime);
		void SetPosition(const CU::Vector3f& aPosition);

	private:

		void CreateVertexBuffer();
		void UpdateVertexBuffer();

		void UpdateEmitter(float aDeltaTime);
		void UpdateParticle(float aDeltaTime);

		void EmittParticle();

		CU::GrowingArray<LogicalParticle> myLogicalParticles;
		CU::GrowingArray<GraphicalParticle> myGraphicalParticles;

		CU::Vector3f myDiffColor;

		CU::Matrix44f myOrientation;

		EmitterData myEmitterData;
		VertexBufferWrapper *myVertexWrapper;

		float myEmissionTime;
		int myParticleIndex;

	};
}
