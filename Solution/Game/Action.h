#pragma once
class Action
{
public:
	virtual ~Action();

	virtual void Start() = 0;
};

