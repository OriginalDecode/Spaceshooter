#pragma once
#include "Subscriber.h"
#include <StaticArray.h>
#define PREALLOCATED_EMITTER_SIZE 10
#define EXPLOSION_DATA_SIZE 6

class ParticleEmitterComponent;

namespace Prism
{
	class ParticleEmitterInstance;
	class Camera;
};

struct ExplosionData
{
	ExplosionData(std::string aType);
	~ExplosionData();

	CU::StaticArray<Prism::ParticleEmitterInstance*, PREALLOCATED_EMITTER_SIZE> myEmitterA;
	CU::StaticArray<Prism::ParticleEmitterInstance*, PREALLOCATED_EMITTER_SIZE> myEmitterB;
	CU::StaticArray<Prism::ParticleEmitterInstance*, PREALLOCATED_EMITTER_SIZE> myEmitterC;

	std::string myType;

	int myEmitterIndex;
};


class EmitterManager : public Subscriber
{
public:
	EmitterManager();
	~EmitterManager();

	void AddEmitter(ParticleEmitterComponent* anEmitter);


	void UpdateEmitters(float aDeltaTime, CU::Matrix44f aWorldMatrix);
	void RenderEmitters(Prism::Camera* aCamera);

	void ReceiveMessage(const DestroyEmitterMessage& aMessage) override;
	void ReceiveMessage(const SpawnExplosionMessage& aMessage) override;


private:

	void ReadListOfLists(std::string aPath);
	void ReadList(std::string aPath);

	void EnemyExplosion(const SpawnExplosionMessage& aMessage);
	void PropExplosion(const SpawnExplosionMessage& aMessage);
	void AstroidExplosion(const SpawnExplosionMessage& aMessage);
	void RocketExplosion(const SpawnExplosionMessage& aMessage);
	void OnHitEffect(const SpawnExplosionMessage& aMessage);
	void OnAstroidHitEffect(const SpawnExplosionMessage& aMessage);

	CU::GrowingArray<ParticleEmitterComponent*> myEmitters;
	CU::GrowingArray<std::string> myXMLPaths;

	CU::StaticArray<ExplosionData*, EXPLOSION_DATA_SIZE> myExplosions;
	
	enum class eExplosionID
	{
		ENEMY_EXPLOSION,
		PROP_EXPLOSION,
		ASTROID_EXPLOSION,
		ROCKET_EXPLOSION,
		ONHIT_EFFECT,
		ON_ASTROID_HIT_EFFECT,
		COUNT
	};

	enum class eTYPE
	{
		EXPLOSION,
		SMOKE,
		SPARK,
		COUNT
	};
};

