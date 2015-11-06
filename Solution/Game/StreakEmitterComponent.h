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

	void AddStreak(const CU::Matrix44f aOrientation);

	//void ReceiveNote(const EmitterNote& aNote) override;


private:
	struct Emitter
	{
		CU::Matrix44f myOrientation;
		Prism::StreakEmitterInstance* myEmitter;
	};

	CU::GrowingArray<Emitter> myEmitters;
	
	Prism::StreakEmitterInstance* myEmitterInstance;

	std::string myXMLPath;
};



inline eComponentType StreakEmitterComponent::GetType()
{
	return eComponentType::STREAK_EMITTER;
}

inline void StreakEmitterComponent::AddStreak(const CU::Matrix44f aOrientation)
{
	Emitter emitter;
	emitter.myEmitter = myEmitterInstance;
	emitter.myOrientation = aOrientation;
	myEmitters.Add(emitter);
}