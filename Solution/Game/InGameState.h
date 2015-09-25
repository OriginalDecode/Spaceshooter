#pragma once
#include "GameState.h"
#include <Matrix.h>
#include <GrowingArray.h>

namespace CommonUtilities
{
	class InputWrapper;
}

class BulletManager;
class Level;

class InGameState : public GameState
{
public:
	InGameState();
	~InGameState();

	void InitState(CU::InputWrapper* anInputWrapper) override;
	void EndState() override;

	const eStateStatus Update() override;
	void Render() override;
	void ResumeState() override;

	void OnResize(int aWidth, int aHeight) override;

private:

	void LogicUpdate(const float aDeltaTime);

	bool CheckCollision();

	BulletManager* myBulletManager;
	Level* myLevel;
};

