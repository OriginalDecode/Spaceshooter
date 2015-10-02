#pragma once
#include "Component.h"

namespace Prism
{
	class Instance;
	struct MeshData;
	class Camera;
};

class GraphicsComponent : public Component
{
public:
	GraphicsComponent(Entity& aEntity);

	void Init(const char* aModelPath, const char* aEffectPath);
	void InitGeometry(const Prism::MeshData& aMeshData);
	void InitCube(float aWidth, float aHeight, float aDepth);
	void Update(float aDeltaTime);
	Prism::Instance* GetInstance();
	static int GetID();

	void SetPosition(const CU::Vector3<float>& aPosition);

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

