#pragma once
#include <functional>   // std::bind

namespace Prism
{
	namespace Audio
	{
		class WwiseManager;

		class AudioInterface
		{
		public:
			static AudioInterface* GetInstance(){ return myInstance; }
			static void CreateInstance(){ myInstance = new AudioInterface(); }
			static void Destroy(){ delete myInstance; myInstance = nullptr; }

			bool Init(const char* aInitBank);
			bool LoadBank(const char* aBankPath);
			void UnLoadBank(const char* aBankPath);
			void PostEvent(const char* aEvent, int anObjectID);
			void Update();
			void SetRTPC(const char* aRTPC, int aValue, int anObjectID);
			void SetPosition(float aX, float aY, float aZ, int aObjectID);
			void SetListenerPosition(float aX, float aY, float aZ);

			void RegisterObject(int anObjectID);
			void UnRegisterObject(int aObjectID);
			typedef void(*callback_function)(const char*);
			void SetErrorCallBack(callback_function aErrorCallback);

			int GetUniqueID();
		private:
			static AudioInterface* myInstance;
			AudioInterface();
			~AudioInterface();


			WwiseManager* myWwiseManager;
		};
	}
}