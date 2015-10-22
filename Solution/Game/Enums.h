#pragma once

enum class eMessageType
{
	ACTIVATE_BULLET,
	GAME_STATE,
	SPAWN_ENEMY,
	START_EVENT,
	ENEMY_KILLED,
	POWER_UP,
	CONVERSATION,
	EVENT_QUEUE_EMPTY,
	DEFEND,
	RESIZE,
	BULLET_COLLISION_TO_GUI,
	COUNT,
};

enum class eBulletType
{
	MACHINGUN_BULLET_LEVEL_1,
	MACHINGUN_BULLET_LEVEL_2,
	MACHINGUN_BULLET_LEVEL_3,
	SHOTGUN_BULLET_LEVEL_1,
	SHOTGUN_BULLET_LEVEL_2,
	SHOTGUN_BULLET_LEVEL_3,
	ROCKET_MISSILE_LEVEL_1,
	ROCKET_MISSILE_LEVEL_2,
	ROCKET_MISSILE_LEVEL_3,
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
	DEFENDABLE = 128
};

enum class ePowerUpType
{
	NO_POWERUP,
	FIRERATEBOOST,
	SHIELDBOOST,
	HEALTHKIT,
	WEAPON_UPGRADE,
	EMP,
	HOMING,
	INVULNERABLITY
};

enum class eMissionType
{
	WAYPOINT,
	KILL_ALL,
};

enum class eMissionCategory
{
	REQUIRED,
	NOT_REQUIRED,
	DUMMY
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
	EMITTER,
	_COUNT,
};

enum class eAITargetPositionMode
{
	NOT_USED,
	KEEP_DISTANCE,
	ESCAPE_THEN_RETURN,
	KAMIKAZE,
	MINE
};