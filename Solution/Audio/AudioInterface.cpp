#include "AudioInterface.h"
#include "WwiseManager.h"


namespace Prism
{
	namespace Audio
	{
		AudioInterface* AudioInterface::myInstance = nullptr;
		AudioInterface::AudioInterface()
			:myWwiseManager(nullptr)
		{
		}

		AudioInterface::~AudioInterface()
		{
			delete myWwiseManager;
		}

		bool AudioInterface::Init(const char* aInitBank)
		{
			myWwiseManager = new WwiseManager();
			return myWwiseManager->Init(aInitBank);
		}

		void AudioInterface::Update()
		{
			if (myWwiseManager)
			{
				myWwiseManager->Update();
			}

		}

		void AudioInterface::SetRTPC(const char* aRTPC, int aValue, int anObjectID)
		{
			if (myWwiseManager)
			{
				myWwiseManager->SetRTPC(aRTPC, aValue, anObjectID);
			}
		}

		void AudioInterface::SetPosition(float aX, float aY, float aZ, int aObjectID)
		{
			if (myWwiseManager)
			{
				myWwiseManager->SetPosition(aX, aY, aZ, aObjectID);
			}
		}

		void AudioInterface::SetListenerPosition(float aX, float aY, float aZ)
		{
			if (myWwiseManager)
			{
				myWwiseManager->SetListenerPosition(aX, aY, aZ);
			}
		}

		bool AudioInterface::LoadBank(const char* aBankPath)
		{
			if (myWwiseManager)
			{
				return myWwiseManager->LoadBank(aBankPath);
			}
			return false;
		}

		void AudioInterface::UnLoadBank(const char* aBankPath)
		{
			if (myWwiseManager)
			{
				return myWwiseManager->UnLoadBank(aBankPath);
			}
		}

		void AudioInterface::PostEvent(const char* aEvent, int anObjectID)
		{
			if (myWwiseManager)
			{
				return myWwiseManager->PostEvent(aEvent, anObjectID);
			}
		}

		void AudioInterface::SetErrorCallBack(callback_function aErrorCallback)
		{
			if (myWwiseManager)
			{
				return myWwiseManager->SetErrorCallBack(aErrorCallback);
			}
		}

		void AudioInterface::RegisterObject(int anObjectID)
		{
			if (myWwiseManager)
			{
				return myWwiseManager->RegisterObject(anObjectID);
			}
		}
	}
}