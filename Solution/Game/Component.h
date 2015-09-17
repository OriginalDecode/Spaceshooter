#pragma once
class Component
{
public:

	virtual void Init();
	virtual void Update(float aDeltaTime);
	static int GetID();

protected:
	static int myID;
};

inline int Component::GetID()
{
	return myID;
}