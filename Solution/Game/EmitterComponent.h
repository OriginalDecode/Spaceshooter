#pragma once
#include "Component.h"

namespace Prism
{
	class EmitterInstance;
}

class EmitterComponent : public Component
{
public:
	EmitterComponent(Entity& aEntity);
	~EmitterComponent();
	void Init(std::string aPath);
	void Update(float aDeltaTime) override;
	void Render();
	static eComponentType GetType();
	int GetEmitterCount();
	Prism::EmitterInstance* GetEmitter();
private:

	CU::Matrix44f myOrientation;

	CU::Vector3f myPosition;
	Prism::EmitterInstance* myEmitter;
	std::string myXMLPath;
	static int myEmitterCount; //Emitter count duh.
};

