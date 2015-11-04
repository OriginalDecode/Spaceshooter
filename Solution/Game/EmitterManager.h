#pragma once
#include "Subscriber.h"
#include <StaticArray.h>
#define PREALLOCATED_EMITTER_SIZE 10

class ParticleEmitterComponent;

namespace Prism
{
	class ParticleEmitterInstance;
	class Camera;
};


class EmitterManager : public Subscriber
{
public:
	EmitterManager();
	~EmitterManager();

	void AddEmitter(ParticleEmitterComponent* anEmitter);


	void UpdateEmitters(float aDeltaTime, CU::Matrix44f aWorldMatrix);
	void RenderEmitters(Prism::Camera* aCamera);

	void ReceiveMessage(const DestroyEmitterMessage& aMessage) override;
	void ReceiveMessage(const SpawnExplosionMessage& aMessage) override;


private:

	CU::GrowingArray<ParticleEmitterComponent*> myEmitters;


	CU::StaticArray<Prism::ParticleEmitterInstance*, PREALLOCATED_EMITTER_SIZE> myFireExplosion;
	//CU::StaticArray<Prism::ParticleEmitterInstance*, 10> mySmokeExplosion;
	//CU::StaticArray<Prism::ParticleEmitterInstance*, 10> mySparkExplosion;
	//CU::StaticArray<Prism::ParticleEmitterInstance*, 10> mySparkExplosion;
	//
	//
	//CU::StaticArray<Prism::ParticleEmitterInstance*, 10> myFireExplosion;
	//CU::StaticArray<Prism::ParticleEmitterInstance*, 10> mySmokeExplosion;
	//CU::StaticArray<Prism::ParticleEmitterInstance*, 10> mySparkExplosion;
	//
	//CU::StaticArray<Prism::ParticleEmitterInstance*, 10> myFireExplosion;
	//CU::StaticArray<Prism::ParticleEmitterInstance*, 10> mySmokeExplosion;
	//CU::StaticArray<Prism::ParticleEmitterInstance*, 10> mySparkExplosion;


	unsigned int myEmitterIndex;
};

