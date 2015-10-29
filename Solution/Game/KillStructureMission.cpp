#include "stdafx.h"
//#include "DefendMessage.h"
//#include "KillStructureMission.h"
//#include "Engine.h"
//#include "Entity.h"
//#include "PostMaster.h"
//#include <sstream>
//#include <XMLReader.h>
//
//
//KillStructureMission::KillStructureMission(XMLReader& aReader, tinyxml2::XMLElement* aElement)
//	: Mission(aReader, aElement)
//	, myStructuresToKillNames(4)
//	, myStructuresToKill(4)
//{
//	std::string structureName;
//	for (tinyxml2::XMLElement* element = aReader.ForceFindFirstChild(aElement, "structure"); element != nullptr;
//		element = aReader.FindNextElement(element, "structure"))
//	{
//		aReader.ForceReadAttribute(element, "structureName", structureName);
//		structureName = CU::ToLower(structureName);
//		myStructuresToKillNames.Add(structureName);
//
//	}
//}
//
//KillStructureMission::~KillStructureMission()
//{
//	if (PostMaster::GetInstance()->IsSubscribed(eMessageType::DEFEND, this) == true)
//	{
//		PostMaster::GetInstance()->UnSubscribe(eMessageType::DEFEND, this);
//	}
//}
//
//void KillStructureMission::Start()
//{
//	PostMaster::GetInstance()->Subscribe(eMessageType::DEFEND, this);
//	for (int i = 0; i < myStructuresToKillNames.Size(); ++i)
//	{
//		//PostMaster::GetInstance()->SendMessage<KillStructureMessage>(KillStructureMessage(KillStructureMessage::eType::NAME, myStructuresToKillNames[i]));
//	}
//}
//
//bool KillStructureMission::Update(float aDeltaTime, int aMissionIndex, eMissionCategory aMissionCategory)
//{
//	DL_ASSERT_EXP(myEntityToDefend != nullptr, "Could not find entity to defend: " + myNameToDefend);
//
//	std::stringstream ss;
//	ss.precision(2);
//
//		ss << "Current mission: KILL STRUCUTRES";
//	}
//
//	if (aMissionCategory == eMissionCategory::NOT_REQUIRED)
//	{
//		ss << " (Optional)";
//	}
//
//	Prism::Engine* engine = Prism::Engine::GetInstance();
//	CU::Vector2<float> screenCenter(engine->GetWindowSize().x * 0.5f, engine->GetWindowSize().y * 0.5f);
//
//	engine->PrintDebugText(ss.str(), { screenCenter.x - 300, (-(screenCenter.y) + screenCenter.y * 0.5f) - aMissionIndex * 25.f });
//
//	return myRealTime <= 0.f;
//}
//
//void KillStructureMission::End()
//{
//	myRealTime = 0.f;
//	myVisualTime = 0.f;
//	PostMaster::GetInstance()->SendMessage<DefendMessage>(DefendMessage(DefendMessage::eType::COMPLETE, myNameToDefend));
//	PostMaster::GetInstance()->UnSubscribe(eMessageType::DEFEND, this);
//}
//
//void KillStructureMission::ReceiveMessage(const KillStructureMessage& aMessage)
//{
//	if (aMessage.myType == DefendMessage::eType::ENTITY)
//	{
//		myEntityToDefend = aMessage.myEntity;
//	}
//}