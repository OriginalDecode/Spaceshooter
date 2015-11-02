#pragma once
#include "Subscriber.h"

class ParticleEmitterComponent;

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

};

