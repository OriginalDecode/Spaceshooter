#pragma once

namespace Prism
{
	class Sprite;
}

class PowerUpGUIIcon
{
public:
	PowerUpGUIIcon();
	~PowerUpGUIIcon();

	void SetActive(float aDuration);
	void Update(float aDeltaTime);

private:
	Prism::Sprite* myActiveIcon;
	Prism::Sprite* myInactiveIcon;
	float myActiveDuration;
	CU::Vector2<float> myPosition;
	bool myIsActive;

};

