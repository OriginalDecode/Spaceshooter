#pragma once
#include <unordered_map>
#undef SendMessage

namespace Prism
{
	class Camera;
}

class Component;


class Entity
{
public:
	Entity();
	~Entity();

	virtual void Update(float aDeltaTime);

	template <typename T>
	T* AddComponent();

	template <typename T>
	T* GetComponent();

	template <typename T>
	void SendMessage(const T& aMessage);



	CU::Matrix44<float> myOrientation;
	Prism::Camera* myCamera;

	bool GetAlive() const;
	void Kill();
private:
	std::unordered_map<int, Component*> myComponents;
	bool myAlive;
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

template <typename T>
void Entity::SendMessage(const T& aMessage)
{
	for (auto it = myComponents.begin(); it != myComponents.end(); ++it)
	{
		it->second->ReceiveMessage(aMessage);
	}
}

inline bool Entity::GetAlive() const
{
	return myAlive;
}

