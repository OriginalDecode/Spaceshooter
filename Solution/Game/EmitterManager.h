#pragma once
#include "Subscriber.h"

class EmitterComponent;

class EmitterManager : public Subscriber
{
public:
	EmitterManager();
	~EmitterManager();

	void AddEmitter(EmitterComponent* anEmitter);


	void UpdateEmitters(float aDeltaTime);
	void RenderEmitters();

	void ReceiveMessage(const DestroyEmitterMessage& aMessage) override;



private:


	CU::GrowingArray<EmitterComponent*> myEmitters;

};

