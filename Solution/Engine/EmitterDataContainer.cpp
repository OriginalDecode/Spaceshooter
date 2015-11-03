#include "stdafx.h"
#include "EmitterDataContainer.h"

#include "ParticleEmitterData.h"
#include "StreakEmitterData.h"

namespace Prism
{
	EmitterDataContainer::~EmitterDataContainer()
	{
		for (auto it = myParticleData.begin(); it != myParticleData.end(); ++it)
		{
			it->second->Release(); //Yeah, release the inputlayout. lel kek bur.
			delete it->second;
			it->second = nullptr;
		}

		for (auto it = myStreakData.begin(); it != myStreakData.end(); ++it)
		{
			delete it->second;
			it->second = nullptr;
		}

	}

	ParticleEmitterData* EmitterDataContainer::GetParticleData(const std::string& aFilePath)
	{
		auto it = myParticleData.find(aFilePath);

		if (it == myParticleData.end())
		{
			LoadParticleData(aFilePath);
		}

		return myParticleData[aFilePath];
	}
		
	StreakEmitterData* EmitterDataContainer::GetStreakData(const std::string& aFilePath)
	{
		auto it = myStreakData.find(aFilePath);

		if (it == myStreakData.end())
		{
			LoadParticleData(aFilePath);
		}

		return myStreakData[aFilePath];
	}

	void EmitterDataContainer::LoadParticleData(const std::string& aFilePath)
	{
		ParticleEmitterData* newData = new ParticleEmitterData();

		newData->LoadDataFile(aFilePath.c_str());
		DL_ASSERT_EXP(newData != nullptr, "Failed to load data. newData became nullptr.");

		myParticleData[aFilePath] = newData;

	}
	
	void EmitterDataContainer::LoadStreakData(const std::string& aFilePath)
	{
		StreakEmitterData* newData = new StreakEmitterData();

		newData->LoadDataFile(aFilePath.c_str());
		DL_ASSERT_EXP(newData != nullptr, "Failed to load data. newData became nullptr.");

		myStreakData[aFilePath] = newData;
	}
}

 