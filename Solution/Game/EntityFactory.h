#pragma once

class Entity;

namespace tinyxml2
{
	class XMLElement;
}

enum class eEntityDataGraphicsType
{
	MODEL,
	CUBE
};

struct EntityData
{
	EntityData();

	Entity* myEntity;

	std::string myEffectFile;
	std::string myModelFile;
	std::string myTargetName;

	float myCollisionSphereRadius;
	float myDepth;
	float myHeight;
	float myWidth;

	int myChanceToFollow;

	eEntityDataGraphicsType myGraphicsType;
};

class EntityFactory
{
public:
	void LoadEntites(const std::string& aEntityRootPath);

	//Entity* GetEntity(const std::string& aEntityTag);
	void CopyEntity(Entity* aTargetEntity, const std::string& aEntityTag);
private:
	void LoadEntity(const std::string& aEntityPath);

	void LoadAIComponent(EntityData& aEntityToAddTo, tinyxml2::XMLElement* aAIComponentElement);
	void LoadGraphicsComponent(EntityData& aEntityToAddTo, tinyxml2::XMLElement* aGraphicsComponentElement);
	void LoadShootingComponent(EntityData& aEntityToAddTo, tinyxml2::XMLElement* aShootingComponenetElement);
	void LoadCollisionComponent(EntityData& aEntityToAddTo, tinyxml2::XMLElement* aCollisionComponenetElement);

	std::unordered_map<std::string, EntityData> myEntities;
};

