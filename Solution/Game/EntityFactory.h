#pragma once

class Entity;
class XMLReader;

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
	~EntityFactory();

	void LoadEntites(const std::string& aEntityRootPath);

	void CopyEntity(Entity* aTargetEntity, const std::string& aEntityTag);
private:
	void LoadEntity(const std::string& aEntityPath);

	void LoadAIComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aAIComponentElement);
	void LoadGraphicsComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aGraphicsComponentElement);
	void LoadShootingComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aShootingComponenetElement);
	void LoadCollisionComponent(EntityData& aEntityToAddTo, XMLReader& aDocument, tinyxml2::XMLElement* aCollisionComponenetElement);

	std::unordered_map<std::string, EntityData> myEntities;
};

