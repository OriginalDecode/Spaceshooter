#pragma once
#define TIME_FUNCTION Prism::Debug_BlockTimer funcTimer__(__FUNCTION__);
#define BEGIN_TIME_BLOCK(NAME) Prism::Engine::GetInstance()->GetDebugDisplay()->StartFunctionTimer(NAME);
#define END_TIME_BLOCK(NAME) Prism::Engine::GetInstance()->GetDebugDisplay()->EndFunctionTimer(NAME);


#include <bitset>
#include "Debug_BlockTimer.h"
#include "GraphRenderer.h"
#include <sstream>
#include <unordered_map>

namespace CommonUtilities
{
	class InputWrapper;
}

namespace Prism
{
	class Camera;
	class Font;
	class Text;

	class FrameTimeDebugger;

	class DebugDataDisplay
	{
	public:
		DebugDataDisplay();

		void Init();

		void StartFunctionTimer(const std::string& aFunc);
		void EndFunctionTimer(const std::string& aFunc);

		void RecordFrameTime(const float aDeltaTime);
		void Render(const Camera& aCamera);

		void ToggleFunctionTimers();
		void ToggleMemoryUsage();
		void ToggleCPUUsage();
		void ToggleFrameTime();

		void Update(const CU::InputWrapper& aInputWrapper);

	private:
		enum eBitSetEnum
		{
			FUNCTION_TIMERS,
			MEMORY_USAGE,
			CPU_USAGE,
			FRAME_TIME,
			NEW_GRAPH_DATA,
			count
		};


		void RenderFunctionTimers(const Camera& aCamera);
		void RenderMemoryUsage(const Camera& aCamera);
		void RenderCPUUsage(const Camera& aCamera);
		void RenderFrameTime(const Camera& aCamera);

		Text* myText;

		std::bitset<eBitSetEnum::count> myBoolContainer;

		CU::Vector2<float> myFunctionTimersStartPos;
		CU::Vector2<float> myMemUsageStartPos;
		CU::Vector2<float> myCPUUSageStartPos;
		CU::Vector2<float> myFrameTimeStartPos;
		float myTextScale;
		float myLastDeltaTime;
		std::stringstream myStringStream;

		FrameTimeDebugger* myFrameDebugger;
	};
}

inline void Prism::DebugDataDisplay::ToggleFunctionTimers()
{
	myBoolContainer[eBitSetEnum::FUNCTION_TIMERS].flip();
}

inline void Prism::DebugDataDisplay::ToggleMemoryUsage()
{
	myBoolContainer[eBitSetEnum::MEMORY_USAGE].flip();
}

inline void Prism::DebugDataDisplay::ToggleCPUUsage()
{
	myBoolContainer[eBitSetEnum::CPU_USAGE].flip();
}

inline void Prism::DebugDataDisplay::ToggleFrameTime()
{
	myBoolContainer[eBitSetEnum::FRAME_TIME].flip();
}

