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
		void Initiate(EmitterData& someData);
		void Render(Camera* aCamera);
		void Update();

	private:

		void CreateVertexBuffer();
		void UpdateVertexBuffer();

		void UpdateEmitter();
		void UpdateParticle();

		void EmittParticle();

		CU::GrowingArray<ParticleInstance> myParticles;
		CU::Matrix44f myOrientation;

		EmitterData myData;
		VertexBufferWrapper *myVertexWrapper;

		float myEmissionTime;

	};
}
