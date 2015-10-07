#pragma once
class Mission
{
public:
	Mission();
	virtual ~Mission();

	virtual bool Update(float aDeltaTime) = 0;
	virtual void Start();
	virtual void End();

};

