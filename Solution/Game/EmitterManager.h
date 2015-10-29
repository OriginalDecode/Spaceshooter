#pragma once

class EmitterComponent;

class EmitterManager
{
public:
	EmitterManager();
	~EmitterManager();

	void AddEmitter(EmitterComponent* anEmitter);


	void UpdateEmitters(float aDeltaTime);
	void RenderEmitters();


private:


	CU::GrowingArray<EmitterComponent*> myEmitters;

};

