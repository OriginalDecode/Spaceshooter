#include "stdafx.h"

#include "AIComponent.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "GraphicsComponent.h"
#include <Instance.h>
#include <XMLReader.h>

EntityData::EntityData()
	: myEntity(new Entity())
{
}

void EntityFactory::LoadEntites(const std::string& aEntityRootPath)
{
	XMLReader rootDocument;
	rootDocument.OpenDocument(aEntityRootPath);
	tinyxml2::XMLElement* rootElement = rootDocument.FindFirstChild("root");

	for (tinyxml2::XMLElement* e = rootDocument.FindFirstChild(rootElement); e != nullptr;
		e = rootDocument.FindNextElement(e))
	{
		std::string entityPath = "";
		rootDocument.ForceReadAttribute(e, "src", entityPath);
		if (entityPath != "")
		{
			LoadEntity(entityPath);
		}
	}

	rootDocument.CloseDocument();
}

void EntityFactory::LoadEntity(const std::string& aEntityPath)
{
	XMLReader entityDocument;
	entityDocument.OpenDocument(aEntityPath);

	EntityData newEntity;

	tinyxml2::XMLElement* rootElement = entityDocument.FindFirstChild("Entity");

	std::string entityName = "";
	entityDocument.ForceReadAttribute(rootElement, "name", entityName);

	for (tinyxml2::XMLElement* e = entityDocument.FindFirstChild(rootElement); e != nullptr;
		e = entityDocument.FindNextElement(e))
	{
		std::string childName = e->Name();
		if (std::strcmp(e->Name(), "GraphicsComponent") == 0)
		{
			LoadGraphicsComponent(newEntity, e);
		}
		else if (std::strcmp(e->Name(), "AIComponent") == 0)
		{
			LoadAIComponent(newEntity, e);
		}
	}
	if (entityName != "")
	{
		myEntities.insert(std::pair<std::string, EntityData>(entityName, newEntity));
	}

	entityDocument.CloseDocument();
}

void EntityFactory::LoadAIComponent(EntityData& aEntityToAddTo, tinyxml2::XMLElement* aAIComponentElement)
{
	aEntityToAddTo;
	aAIComponentElement;
}

void EntityFactory::LoadGraphicsComponent(EntityData& aEntityToAddTo, tinyxml2::XMLElement* aGraphicsComponentElement)
{
	aEntityToAddTo.myEntity->AddComponent<GraphicsComponent>();

	for (tinyxml2::XMLElement* e = aGraphicsComponentElement->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (std::strcmp(e->Name(), "Model") == 0)
		{
			std::string modelFile = e->Attribute("modelFile");
			std::string effectFile = e->Attribute("effectFile");

			aEntityToAddTo.myModelFile = modelFile;
			aEntityToAddTo.myEffectFile = effectFile;

			aEntityToAddTo.myGraphicsType = eEntityDataGraphicsType::MODEL;
		}
		else if (std::strcmp(e->Name(), "Cube") == 0)
		{
			float width = e->FloatAttribute("width");
			float height = e->FloatAttribute("height");
			float depth = e->FloatAttribute("depth");

			aEntityToAddTo.myWidth = width;
			aEntityToAddTo.myHeight = height;
			aEntityToAddTo.myDepth = depth;

			aEntityToAddTo.myGraphicsType = eEntityDataGraphicsType::CUBE;
		}
	}
}
//
//Entity* EntityFactory::GetEntity(const std::string& aEntityTag)
//{
//	auto it = myEntities.find(aEntityTag);
//
//	return it->second;
//}

void EntityFactory::CopyEntity(Entity* aTargetEntity, const std::string& aEntityTag)
{
	auto it = myEntities.find(aEntityTag);
	Entity* sourceEntity = it->second.myEntity;

	if (sourceEntity->GetComponent<GraphicsComponent>() != nullptr)
	{
		aTargetEntity->AddComponent<GraphicsComponent>()->GetInstance();
		switch (it->second.myGraphicsType)
		{
		case eEntityDataGraphicsType::MODEL:
			aTargetEntity->GetComponent<GraphicsComponent>()->Init(it->second.myModelFile.c_str(),
				it->second.myEffectFile.c_str());
			break;
		case eEntityDataGraphicsType::CUBE:
			aTargetEntity->GetComponent<GraphicsComponent>()->InitCube(it->second.myWidth,
				it->second.myHeight, it->second.myDepth);
			break;
		default:
			break;
		}
	}
}