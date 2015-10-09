#include "AudioInterface.h"
#include "WwiseManager.h"


CAudioInterface* CAudioInterface::myInstance = nullptr;
CAudioInterface::CAudioInterface()
	:myWwiseManager(nullptr)
{
}

CAudioInterface::~CAudioInterface()
{
	delete myWwiseManager;
}

bool CAudioInterface::Init(const char* aInitBank)
{
	myWwiseManager = new CWwiseManager();
	return myWwiseManager->Init(aInitBank);
}

void CAudioInterface::Update()
{
	if (myWwiseManager)
	{
		myWwiseManager->Update();
	}
		
}

void CAudioInterface::SetRTPC(const char* aRTPC, int aValue, int anObjectID)
{
	if (myWwiseManager)
	{
		myWwiseManager->SetRTPC(aRTPC, aValue, anObjectID);
	}
}

void CAudioInterface::SetPosition(float aX, float aY, float aZ, int aObjectID)
{
	if (myWwiseManager)
	{
		myWwiseManager->SetPosition(aX, aY, aZ, aObjectID);
	}
}

void CAudioInterface::SetListenerPosition(float aX, float aY, float aZ)
{
	if (myWwiseManager)
	{
		myWwiseManager->SetListenerPosition(aX, aY, aZ);
	}
}

bool CAudioInterface::LoadBank(const char* aBankPath)
{
	if (myWwiseManager)
	{
		return myWwiseManager->LoadBank(aBankPath);
	}
	return false;
}

void CAudioInterface::UnLoadBank(const char* aBankPath)
{
	if (myWwiseManager)
	{
		return myWwiseManager->UnLoadBank(aBankPath);
	}
}

void CAudioInterface::PostEvent(const char* aEvent, int anObjectID)
{
	if (myWwiseManager)
	{
		return myWwiseManager->PostEvent(aEvent, anObjectID);
	}
}

void CAudioInterface::SetErrorCallBack(callback_function aErrorCallback)
{
	if (myWwiseManager)
	{
		return myWwiseManager->SetErrorCallBack(aErrorCallback);
	}
}

void CAudioInterface::RegisterObject(int anObjectID)
{
	if (myWwiseManager)
	{
		return myWwiseManager->RegisterObject(anObjectID);
	}
}