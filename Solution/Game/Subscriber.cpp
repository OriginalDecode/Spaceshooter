#include "stdafx.h"
#include "Subscriber.h"
#include "Message.h"

Subscriber::Subscriber()
{
}


Subscriber::~Subscriber()
{
}

void Subscriber::ReceiveMessage(const Message&)
{
}

void Subscriber::ReceiveMessage(const BulletMessage&)
{
}

void Subscriber::ReceiveMessage(const GameStateMessage&)
{
}

void Subscriber::ReceiveMessage(const SpawnEnemyMessage&)
{
}
