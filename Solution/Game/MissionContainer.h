#pragma once
#include <GrowingArray.h>
#include "Mission.h"
#include "Subscriber.h"

class Entity;
class Level;
class XMLReader;
namespace tinyxml2
{
	class XMLElement;
}

class MissionContainer : public Mission, public Subscriber
{
public:
	MissionContainer(Level& aLevel, Entity& aPlayer, XMLReader& aReader, tinyxml2::XMLElement* aElement);
	~MissionContainer();

	bool Update(float aDeltaTime) override;
	void Start() override;
	void End() override;

	void ReceiveMessage(const EventQueueEmptyMessage&) override;
private:
	CU::GrowingArray<Mission*> myRequiredMissions;
	CU::GrowingArray<Mission*> myOptionalMissions;
	CU::GrowingArray<Mission*> myActiveMissions;
	CU::GrowingArray<Mission*> myEndingMissions;
};

