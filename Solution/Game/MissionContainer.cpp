#include "stdafx.h"
//#include <sstream>
//#include "MissionContainer.h"
//#include "XMLReader.h"
//
//
//MissionContainer::MissionContainer(XMLReader& aReader, tinyxml2::XMLElement* aElement)
//	: Mission(aReader, aElement)
//{
//	tinyxml2::XMLElement* element = aReader.ForceFindFirstChild(aElement, "secondsvisual");
//	aReader.ForceReadAttribute(element, "value", myVisualTimeStart);
//	element = aReader.ForceFindFirstChild(aElement, "secondsreal");
//	aReader.ForceReadAttribute(element, "value", myRealTimeStart);
//}
//
//bool MissionContainer::Update(float aDeltaTime)
//{
//	std::stringstream ss;
//	ss.precision(2);
//	ss << "Current mission: Survive for: " << myVisualTime << " seconds (abort)";
//
//	Prism::Engine* engine = Prism::Engine::GetInstance();
//	CU::Vector2<float> screenCenter(engine->GetWindowSize().x * 0.5f, engine->GetWindowSize().y * 0.5f);
//
//	engine->PrintDebugText(ss.str(), { screenCenter.x - 300, -(screenCenter.y) + screenCenter.y * 0.5f });
//	myVisualTime -= aDeltaTime;
//	myRealTime -= aDeltaTime;
//	return myRealTime <= 0;
//}
//
//void MissionContainer::Start()
//{
//	myVisualTime = myVisualTimeStart;
//	myRealTime = myRealTimeStart;
//}
//
//void MissionContainer::End()
//{
//	myVisualTime = 0.f;
//	myRealTime = 0.f;
//}
