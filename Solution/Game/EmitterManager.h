#pragma once
#include "Subscriber.h"
#include <StaticArray.h>
#define PREALLOCATED_EMITTER_SIZE 10
#define EXPLOSION_DATA_SIZE 5

class ParticleEmitterComponent;

namespace Prism
{
	class ParticleEmitterInstance;
	class Camera;
};

struct ExplosionData
{

#ifdef _DEBUG
	ExplosionData(std::string aType)
		: myType(aType)
		, myEmitterIndex(0)
	{
	}
#endif

	CU::StaticArray<Prism::ParticleEmitterInstance*, PREALLOCATED_EMITTER_SIZE> myFireExplosion;
	CU::StaticArray<Prism::ParticleEmitterInstance*, PREALLOCATED_EMITTER_SIZE> mySmokeExplosion;
	CU::StaticArray<Prism::ParticleEmitterInstance*, PREALLOCATED_EMITTER_SIZE> mySparkExplosion;

#ifdef _DEBUG
	std::string myType;
#endif

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
		COUNT
	};
};

