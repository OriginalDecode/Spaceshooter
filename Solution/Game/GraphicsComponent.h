#pragma once
#include "Component.h"

namespace Prism
{
	class Instance;
};

class GraphicsComponent : public Component
{
public:
	GraphicsComponent();

	void Init(const char* aModelPath, const char* aEffectPath);
	void Update(float aDeltaTime);
	void ReceiveMessage(eMessage aMessage) override;
	Prism::Instance* GetInstance();
	static int GetID();

private:
	Prism::Instance* myInstance;
};

inline Prism::Instance* GraphicsComponent::GetInstance()
{
	return myInstance;
}

inline int GraphicsComponent::GetID()
{
	return 0;
}