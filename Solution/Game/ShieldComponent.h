#pragma once
#include "Component.h"
#include "Enums.h"
class ShieldComponent : public Component
{
public:
	ShieldComponent(Entity& aEntity);
	~ShieldComponent();

	void ReceiveNote(const PowerUpNote& aNote) override;

private:




};

