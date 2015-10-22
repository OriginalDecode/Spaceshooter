#pragma once

#include "Message.h"

class Entity;

class BulletCollisionToGUIMessage : public Message
{
public:
	BulletCollisionToGUIMessage(const Entity& aBullet, const Entity& aEntityCollidedWith);

	const Entity& myBullet;
	const Entity& myEntityCollidedWith;
};


inline BulletCollisionToGUIMessage::BulletCollisionToGUIMessage(const Entity& aBullet, const Entity& aEntityCollidedWith)
	: myBullet(aBullet)
	, myEntityCollidedWith(aEntityCollidedWith)
	, Message(eMessageType::BULLET_COLLISION_TO_GUI)
{
}
