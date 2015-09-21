#pragma once
#include <string>
#include <unordered_map>

namespace Prism
{
	class Effect;
	class EffectContainer
	{
	public:
		Effect* GetEffect(const std::string& aFilePath);

		void Update(const float aDeltaTime);

	private:
		void LoadEffect(const std::string& aFilePath);
		void ReloadEffect(const std::string& aFilePath);

		std::unordered_map<std::string, Effect*> myEffects;
	};
}