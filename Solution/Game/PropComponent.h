#pragma once
#include "Component.h"
#include "Subscriber.h"

struct CollisionNote;
class Entity;

class PropComponent : public Component, public Subscriber
{
public:
	PropComponent(Entity& aEntity);
	~PropComponent();

	void Init(const std::string& aDefendName);

	void ReceiveNote(const CollisionNote& aNote) override;
	void ReceiveMessage(const DefendMessage& aMessage) override;

	const std::string& GetDefendName() const;

	static eComponentType GetType();
private:
	std::string myDefendName;
};

inline eComponentType PropComponent::GetType()
{
	return eComponentType::PROP;
}

inline const std::string& PropComponent::GetDefendName() const
{
	return myDefendName;
}
