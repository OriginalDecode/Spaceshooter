#pragma once
#include "Component.h"

namespace Prism
{
	class StreakEmitterInstance;
}

class StreakEmitterComponent : public Component
{
public:
	StreakEmitterComponent(Entity& aEntity);
	~StreakEmitterComponent();
	void Init(std::string aPath);
	void Update(float aDeltaTime) override;
	void Render();
	static eComponentType GetType();
	int GetEmitterCount();
	Prism::StreakEmitterInstance* GetEmitter();

	//void ReceiveNote(const EmitterNote& aNote) override;


private:

	CU::Matrix44f myOrientation;

	CU::Vector3f myPosition;
	Prism::StreakEmitterInstance* myEmitter;
	std::string myXMLPath;
};



inline eComponentType StreakEmitterComponent::GetType()
{
	return eComponentType::STREAK_EMITTER;
}