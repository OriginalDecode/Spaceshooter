#pragma once
#include "Action.h"

class SpawnEnemy : public Action
{
public:
	SpawnEnemy();
	~SpawnEnemy();

	void Start() override;
};

