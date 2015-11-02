#pragma once
#include "Component.h"

namespace Prism
{
	class ParticleEmitterInstance;
}

class ParticleEmitterComponent : public Component
{
public:
	ParticleEmitterComponent(Entity& aEntity);
	~ParticleEmitterComponent();
	void Init(std::string aPath);
	void Update(float aDeltaTime) override;
	void Render();
	static eComponentType GetType();
	int GetEmitterCount();
	Prism::ParticleEmitterInstance* GetEmitter();

	void ReceiveNote(const EmitterNote& aNote) override;


private:

	CU::Matrix44f myOrientation;

	CU::Vector3f myPosition;
	Prism::ParticleEmitterInstance* myEmitter;
	std::string myXMLPath;
};

