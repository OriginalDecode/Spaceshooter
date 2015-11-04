#pragma once
#include "Subscriber.h"

class ParticleEmitterComponent;

namespace Prism
{
	class ParticleEmitterInstance;
};


class EmitterManager : public Subscriber
{
public:
	EmitterManager();
	~EmitterManager();

	void AddEmitter(ParticleEmitterComponent* anEmitter);


	void UpdateEmitters(float aDeltaTime);
	void RenderEmitters();

	void ReceiveMessage(const DestroyEmitterMessage& aMessage) override;



private:


	CU::GrowingArray<ParticleEmitterComponent*> myEmitters;


	//CU::StaticArray<Prism::ParticleEmitterInstance*, 10> myFireExplosion;
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



};

