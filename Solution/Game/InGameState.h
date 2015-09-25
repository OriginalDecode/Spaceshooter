#pragma once
#include "GameState.h"
#include <Matrix.h>
#include <GrowingArray.h>

namespace CommonUtilities
{
	class InputWrapper;
}

namespace Prism
{
	class Camera;
	class DirectionalLight;
	class Model;
	class PointLight;
	class Scene;
	class Text;
	class Instance;
};

class Entity;
class BulletManager;


class InGameState : public GameState
{
public:
	InGameState();
	~InGameState();

	void InitState() override;
	void EndState() override;

	const eStateStatus& Update() override;
	void Render() override;
	void ResumeState() override;

private:

};

