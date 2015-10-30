#pragma once

#include <Vector.h>

namespace Prism
{
	class Sprite;
}

class PowerUpGUIIcon
{
public:
	PowerUpGUIIcon(const std::string& aActivePath, const std::string& aInactivePath
		, const CU::Vector2<float>& aPosition, const bool& aActive, const float* aDuration = nullptr);
	~PowerUpGUIIcon();

	void Render();

private:
	Prism::Sprite* myActiveIcon;
	Prism::Sprite* myInactiveIcon;
	CU::Vector2<float> myPosition;
	CU::Vector2<float> myTextPosition;
	const bool& myActive;
	const float* myDuration;

};

