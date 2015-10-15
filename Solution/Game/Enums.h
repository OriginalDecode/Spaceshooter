#pragma once

enum class eMessageType
{
	ACTIVATE_BULLET,
	GAME_STATE,
	SPAWN_ENEMY,
	START_EVENT,
};

enum class eBulletType
{
	MACHINGUN_BULLET,
	SNIPER_BULLET,
	PLASMA_BULLET,
	SHOTGUN_BULLET,
	ROCKET_MISSILE,
	COUNT,
};

enum eEntityType //Collision manager needs this as ints!
{
	NOT_USED = -1,
	PLAYER = 1,
	ENEMY = 2,
	PLAYER_BULLET = 4,
	ENEMY_BULLET = 8,
	TRIGGER = 16,
	PROP = 32,
	POWERUP = 64,
};

enum class ePowerUpType
{
	NO_POWERUP,
	FIRERATEBOOST,
	SHIELDBOOST,
	HEALTHKIT,
	EMP
};

enum class eMissionType
{
	WAYPOINT,
	KILL_ALL,
};

enum class eMissionEvent
{
	START,
	END,
};

enum class eGUINoteType
{
	WAYPOINT,
	ENEMY,
	POWERUP,
	STEERING_TARGET,
};

enum class eComponentType
{
	NOT_USED,
	AI,
	BULLET,
	COLLISION,
	CONTROLLER,
	GRAPHICS,
	GUI,
	HEALTH,
	INPUT,
	PHYSICS,
	POWERUP,
	PROP,
	SHEILD,
	SHOOTING,
	WAY_POINT,
	_COUNT,
};