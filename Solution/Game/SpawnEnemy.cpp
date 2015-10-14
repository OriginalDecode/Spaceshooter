#include "stdafx.h"
#include "SpawnEnemy.h"
#include <XMLReader.h>


SpawnEnemy::SpawnEnemy(XMLReader& aReader, tinyxml2::XMLElement* aElement)
{
	std::string type;
	aReader.ForceReadAttribute(aElement, "type", type);

}


SpawnEnemy::~SpawnEnemy()
{
}

void SpawnEnemy::Start()
{

}