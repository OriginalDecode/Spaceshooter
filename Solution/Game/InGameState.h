#pragma once
#include "GameState.h"
#include <Matrix.h>
#include <GrowingArray.h>

namespace CommonUtilities
{
	class InputWrapper;
}

class BulletManager;
class CollisionManager;
class Level;

class InGameState : public GameState
{
public:

	InGameState(CU::InputWrapper* anInputWrapper);
	~InGameState();

	void InitState(StateStackProxy* aStateStackProxy) override;
	void EndState() override;

	const eStateStatus Update() override;
	void Render() override;
	void ResumeState() override;

	void OnResize(int aWidth, int aHeight) override;

private:

	bool CheckCollision();

	BulletManager* myBulletManager;
	CollisionManager* myCollisionManager;
	Level* myLevel;
};

