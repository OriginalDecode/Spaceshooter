#pragma once

class Message;
class BulletMessage;
class GameStateMessage;
class SpawnEnemyMessage;
class StartEventMessage;
class EnemyKilledMessage;
class PowerUpMessage;

class Subscriber
{
public:
	Subscriber();
	~Subscriber();

	virtual void ReceiveMessage(const Message& aMessage);
	virtual void ReceiveMessage(const BulletMessage& aMessage);
	virtual void ReceiveMessage(const GameStateMessage& aMessage);
	virtual void ReceiveMessage(const SpawnEnemyMessage& aMessage);
	virtual void ReceiveMessage(const StartEventMessage& aMessage);
	virtual void ReceiveMessage(const EnemyKilledMessage& aMessage);
	virtual void ReceiveMessage(const PowerUpMessage& aMessage);

};

