#pragma once
#include "Component.h"

namespace Prism
{
	class EmitterInstance;
	class Camera;
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
private:


	Prism::EmitterInstance* myEmitter;
	std::string myXMLPath;
};

