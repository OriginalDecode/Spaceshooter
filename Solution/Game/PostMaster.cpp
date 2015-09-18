#include "stdafx.h"

#include "PostMaster.h"

PostMaster* PostMaster::myInstance = nullptr;
PostMaster::PostMaster()
{
}


PostMaster::~PostMaster()
{
	mySubscribers.clear();
}

PostMaster* PostMaster::GetInstance()
{
	DL_ASSERT_EXP(myInstance != nullptr, "Need to create Postmaster before getting it.");
	return myInstance;
}

void PostMaster::Create()
{
	DL_ASSERT_EXP(myInstance == nullptr, "Postmaster already created.");

	myInstance = new PostMaster();
}

void PostMaster::Destroy()
{
	DL_ASSERT_EXP(myInstance != nullptr, "Need to create Postmaster before destroying it.");

	
	delete myInstance;
	myInstance = nullptr;
}

void PostMaster::Subscribe(const eMessageType aMessageType, Subscriber* aSubscriber, ePriorityLayer aPriority, bool aLetThrough)
{
	auto it = mySubscribers.find(aMessageType);
#ifdef _DEBUG
	if (it != mySubscribers.end())
	{
		for (unsigned int i = 0; i < it->second.Size(); ++i)
		{
			DL_ASSERT_EXP(it->second[i].mySubscriber != aSubscriber, "Tried to add the same subscriber to the same message twice.");
		}
	}
#endif

	SubscriberInfo newSubscriber;
	newSubscriber.mySubscriber = aSubscriber;
	newSubscriber.myPriority = aPriority;
	newSubscriber.myLetThrough = aLetThrough;

	if (it == mySubscribers.end())
	{
		mySubscribers[aMessageType].Init(4);
	}

	if (aPriority == ePriorityLayer::NO_PRIO)
	{
		mySubscribers[aMessageType].Add(newSubscriber);
	}
	else
	{
		mySubscribers[aMessageType].Add(newSubscriber);
		SortSubscribers(mySubscribers[aMessageType]);
	}
}

void PostMaster::UnSubscribe(const eMessageType aMessageType, Subscriber* aSubscriber)
{
	auto it = mySubscribers.find(aMessageType);

	if (it != mySubscribers.end())
	{
		for (unsigned int i = 0; i < unsigned int(mySubscribers[aMessageType].Size()); ++i)
		{
			if (mySubscribers[aMessageType][i].mySubscriber == aSubscriber)
			{
				mySubscribers[aMessageType].RemoveCyclicAtIndex(i);
				break;
			}
		}

		SortSubscribers(mySubscribers[aMessageType]);
	}
}

void PostMaster::UnSubscribe(Subscriber* aSubscriber)
{
	auto it = mySubscribers.begin();

	for (; it != mySubscribers.end(); ++it)
	{
		for (int i = it->second.Size() - 1; i >= 0; --i)
		{
			if (it->second[i].mySubscriber == aSubscriber)
			{
				it->second.RemoveCyclicAtIndex(i);
				break;
			}
		}

		SortSubscribers(it->second);
	}
}

void PostMaster::SortSubscribers(CU::GrowingArray<SubscriberInfo, unsigned int> &aBuffer)
{
	int max = 0;
	if (aBuffer.Size() < 3)
	{
		if (aBuffer.Size() > 1 && (aBuffer[1].myPriority > aBuffer[0].myPriority))
		{
			std::swap(aBuffer[1], aBuffer[0]);
		}
		return;
	}

	for (unsigned int i = 0; i < aBuffer.Size(); ++i)
	{
		if (aBuffer[max].myPriority < aBuffer[i].myPriority)
			max = i;
	}

	std::swap(aBuffer[max], aBuffer[aBuffer.Size() - 1]);
	QuickSort(aBuffer, 0, aBuffer.Size() - 2);
}

void PostMaster::QuickSort(CU::GrowingArray<SubscriberInfo, unsigned int> &aBuffer, const int aStart, const int aEnd)
{
	int lower = aStart + 1;
	int upper = aEnd;

	std::swap(aBuffer[aStart], aBuffer[(aStart + aEnd) / 2]);

	int bound = static_cast<int>(aBuffer[aStart].myPriority);

	while (lower <= upper)
	{
		while (bound > static_cast<int>(aBuffer[lower].myPriority))
			++lower;

		while (bound < static_cast<int>(aBuffer[upper].myPriority))
			--upper;

		if (lower < upper)
			std::swap(aBuffer[lower++], aBuffer[upper--]);
		else
			++lower;
	}

	std::swap(aBuffer[aStart], aBuffer[upper]);

	if (aStart < upper - 1)
		QuickSort(aBuffer, aStart, upper - 1);
	if (upper + 1 < aEnd)
		QuickSort(aBuffer, upper + 1, aEnd);
}