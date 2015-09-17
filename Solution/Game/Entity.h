#pragma once
#include "ComponentEnums.h"
#include <unordered_map>
#undef SendMessage


class Component;

class Entity
{
public:
	virtual void Update(float aDeltaTime);

	template <typename T>
	T* AddComponent();

	template <typename T>
	T* GetComponent();

	void SendMessage(eMessage aMessage);

private:
	std::unordered_map<int, Component*> myComponents;
};

template <typename T>
T* Entity::AddComponent()
{
	T* component = new T();
	component->SetEntity(this);
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