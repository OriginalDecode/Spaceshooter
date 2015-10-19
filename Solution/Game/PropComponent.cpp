#include "stdafx.h"
#include "CollisionNote.h"
#include "DefendMessage.h"
#include "Entity.h"
#include "HealthComponent.h"
#include "PropComponent.h"
#include "ShieldComponent.h"
#include "PhysicsComponent.h"
#include "PostMaster.h"

PropComponent::PropComponent(Entity& aEntity)
	: Component(aEntity)
{
	PostMaster::GetInstance()->Subscribe(eMessageType::DEFEND, this);
}


PropComponent::~PropComponent()
{
	PostMaster::GetInstance()->UnSubscribe(eMessageType::DEFEND, this);
}

void PropComponent::Init(const std::string& aDefendName)
{
	myDefendName = aDefendName;
}

void PropComponent::ReceiveNote(const CollisionNote& aNote)
{
	if (aNote.myEntity.GetAlive() == true)
	{
		if (aNote.myEntity.GetComponent<ShieldComponent>() != nullptr)
		{
			COMPONENT_LOG("Shield component found on entity. (BulletComponent)");
			aNote.myEntity.GetComponent<ShieldComponent>()->DamageShield(10);
		}

		if (aNote.myEntity.GetComponent<ShieldComponent>() == nullptr ||
			aNote.myEntity.GetComponent<ShieldComponent>()->GetCurrentShieldStrength() <= 0)
		{
			COMPONENT_LOG("No shield component found on entity or shield were depleted.");
			aNote.myEntity.GetComponent<HealthComponent>()->RemoveHealth(10);
		}
	}
	// should also bounce of prop
	//aNote.myEntity.GetComponent<PhysicsComponent>()->BounceOff(1);
}

void PropComponent::ReceiveMessage(const DefendMessage& aMessage)
{
	if (aMessage.myType == DefendMessage::eType::NAME && aMessage.myDefendName == myDefendName)
	{
		PostMaster::GetInstance()->SendMessage<DefendMessage>(DefendMessage(DefendMessage::eType::ENTITY, myDefendName, &GetEntity()));
	}
}