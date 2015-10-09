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

		void AudioInterface::PostEvent(const char* aEvent)
		{
			if (myWwiseManager)
			{
				return myWwiseManager->PostEvent(aEvent);
			}
		}

		void AudioInterface::SetErrorCallBack(callback_function aErrorCallback)
		{
			if (myWwiseManager)
			{
				return myWwiseManager->SetErrorCallBack(aErrorCallback);
			}
		}
	}
}