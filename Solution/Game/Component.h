#pragma once
class Component
{
public:

	virtual void Init() = 0;
	virtual void Update(float aDeltaTime) = 0;
	static int GetID();

protected:
	static int myID;
};

inline int Component::GetID()
{
	return myID;
}