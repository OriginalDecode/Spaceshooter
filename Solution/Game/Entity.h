#pragma once
#include "Enums.h"
#include <unordered_map>

namespace Prism
{
	class Camera;
	class Scene;
}

class Component;


class Entity
{
public:
	Entity(eEntityType aType, Prism::Scene& aScene);
	~Entity();

	virtual void Update(float aDeltaTime);

	template <typename T>
	T* AddComponent();

	template <typename T>
	T* GetComponent();

	template <typename T>
	void SendNote(const T& aNote);


	CU::Matrix44<float> myOrientation;
	Prism::Camera* myCamera;

	eEntityType GetType() const;
	bool GetAlive() const;
	void Kill();

	Prism::Scene& GetScene();

private:
	void operator=(Entity&) = delete;
	std::unordered_map<int, Component*> myComponents;
	bool myAlive;
	const eEntityType myType;
	Prism::Scene& myScene;
};

template <typename T>
T* Entity::AddComponent()
{
	T* component = new T(*this);
	myComponents[T::GetID()] = component;
	return component;
}

template <typename T>
T* Entity::GetComponent()
{
	auto it = myComponents.find(T::GetID());

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