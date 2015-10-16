#pragma once

#include "Enums.h"
#include <GrowingArray.h>
#include "Subscriber.h"
#include <unordered_map>
#include <VectorOnStack.h>
#include <Vector.h>

#undef SendMessage

enum class ePriorityLayer
{
	NO_PRIO,
	NORMAL,
	HIGH
};

struct SubscriberInfo
{
	Subscriber* mySubscriber;
	ePriorityLayer myPriority = ePriorityLayer::NO_PRIO;
	bool myLetThrough = true;
};

class PostMaster
{
public:
	static PostMaster* GetInstance();
	static void Create();
	static void Destroy();

	void Subscribe(const eMessageType aMessageType, Subscriber* aSubscriber, ePriorityLayer aPriority = ePriorityLayer::NO_PRIO, bool aLetThrough = true);
	void UnSubscribe(const eMessageType aMessageType, Subscriber* aSubscriber);
	void UnSubscribe(Subscriber* aSubscriber);

	template<typename Message>
	void SendMessage(const Message& aMessage);

private:
	PostMaster();
	~PostMaster();
	static PostMaster* myInstance;
	
	void SortSubscribers(CU::GrowingArray<SubscriberInfo, unsigned int>& aSubscribers);
	void QuickSort(CU::GrowingArray<SubscriberInfo, unsigned int>& aBuffer, const int aStart, const int aEnd);

	std::unordered_map<eMessageType, CU::GrowingArray<SubscriberInfo, unsigned int>> mySubscribers;
};

template<typename Message>
void PostMaster::SendMessage(const Message& aMessage)
{
	auto it = mySubscribers.find(aMessage.GetMessageType());

	if (it != mySubscribers.end())
	{
		CU::GrowingArray<SubscriberInfo, unsigned int>& subscribers = it->second;

		for (unsigned int i = 0; i < subscribers.Size(); ++i)
		{
			subscribers[i].mySubscriber->ReceiveMessage(aMessage);

			if (subscribers[i].myLetThrough == false)
			{
				return;
			}
		}
	}
	else if (aMessage.GetMessageType() != eMessageType::ENEMY_KILLED)
	{
		DL_ASSERT("Message sent without subscriber.");
	}
}