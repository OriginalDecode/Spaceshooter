#pragma once
#include "Component.h"

class Instance;

class GraphicsComponent : public Component
{
public:
	GraphicsComponent();

	void Init(const char* aModelPath, const char* aEffectPath);
	void Update(float aDeltaTime);

	Instance* GetInstance();
	static int GetID();

private:
	Instance* myInstance;
};

inline Instance* GraphicsComponent::GetInstance()
{
	return myInstance;
}

inline int GraphicsComponent::GetID()
{
	return 0;
}