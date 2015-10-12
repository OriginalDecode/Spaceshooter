#pragma once
#include "Enums.h"
#include <unordered_map>

namespace Prism
{
	class Camera;
	class Scene;
	enum class eOctreeType;
}

class Component;


class Entity
{
public:
	Entity(eEntityType aType, Prism::Scene& aScene, Prism::eOctreeType anOctreeType, const std::string& aName = "");
	~Entity();

	virtual void Update(float aDeltaTime);

	template <typename T>
	T* AddComponent();

	template <typename T>
	T* GetComponent();

	template <typename T>
	void SendNote(const T& aNote);

	CU::Matrix44<float> myOrientation;

	eEntityType GetType() const;
	bool GetAlive() const;
	void Kill();
	const std::string& GetName() const;
	void SetName(const std::string& aName);

	Prism::Scene& GetScene();

	void SetShouldRotate(bool aShouldRotate);
	void SetShouldRotateX(bool aShouldRotate);
	void SetShouldRotateY(bool aShouldRotate);
	void SetShouldRotateZ(bool aShouldRotate);

	bool GetShouldRotate();
	bool GetShouldRotateX();
	bool GetShouldRotateY();
	bool GetShouldRotateZ();


	void SetPowerUp(ePowerUpType someType);
	ePowerUpType GetPowerUpType() const;
	Prism::eOctreeType GetOctreeType() const;

private:
	void operator=(Entity&) = delete;
	std::unordered_map<int, Component*> myComponents;
	bool myAlive;
	std::string myName;
	const eEntityType myType;
	Prism::Scene& myScene;
	const Prism::eOctreeType myOctreeType;
	ePowerUpType myPowerUpType;

	bool myShouldRotate;
	bool myRotateX;
	bool myRotateY;
	bool myRotateZ;

};

template <typename T>
T* Entity::AddComponent()
{
	DL_ASSERT_EXP(T::GetType() != eComponentType::NOT_USED, "Tried to add invalid component.");

	T* component = new T(*this);
	myComponents[static_cast<int>(T::GetType())] = component;
	return component;
}

template <typename T>
T* Entity::GetComponent()
{
	auto it = myComponents.find(static_cast<int>(T::GetType()));

	if (it == myComponents.end())
	{
		return nullptr;
	}

	return static_cast<T*>(it->second);
}

template <typename T>
void Entity::SendNote(const T& aMessage)
{
	for (auto it = myComponents.begin(); it != myComponents.end(); ++it)
	{
		it->second->ReceiveNote(aMessage);
	}
}

inline eEntityType Entity::GetType() const
{
	return myType;
}

inline bool Entity::GetAlive() const
{
	return myAlive;
}

inline Prism::Scene& Entity::GetScene()
{
	return myScene;
}

inline const std::string& Entity::GetName() const
{
	return myName;
}

inline void Entity::SetName(const std::string& aName)
{
	myName = aName;
}

inline void Entity::SetPowerUp(ePowerUpType someType)
{
	myPowerUpType = someType;
}

inline ePowerUpType Entity::GetPowerUpType() const
{
	return myPowerUpType;
}

inline Prism::eOctreeType Entity::GetOctreeType() const
{
	return myOctreeType;
}

inline void Entity::SetShouldRotate(bool aShouldRotate)
{
	myShouldRotate = aShouldRotate;
}

inline bool Entity::GetShouldRotate()
{
	return myShouldRotate;
}

inline void Entity::SetShouldRotateX(bool aShouldRotate)
{
	myRotateX = aShouldRotate;
}

inline bool Entity::GetShouldRotateX()
{
	return myRotateX;
}

inline void Entity::SetShouldRotateY(bool aShouldRotate)
{
	myRotateY = aShouldRotate;
}

inline bool Entity::GetShouldRotateY()
{
	return myRotateY;
}

inline void Entity::SetShouldRotateZ(bool aShouldRotate)
{
	myRotateZ = aShouldRotate;
}

inline bool Entity::GetShouldRotateZ()
{
	return myRotateZ;
}