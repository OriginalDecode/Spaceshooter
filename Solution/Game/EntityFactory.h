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

	std::string myModelFile;
	std::string myEffectFile;

	float myWidth;
	float myHeight;
	float myDepth;

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

	std::unordered_map<std::string, EntityData> myEntities;
};

