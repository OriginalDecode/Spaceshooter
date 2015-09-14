#pragma once
#define TIME_FUNCTION Debug_BlockTimer funcTimer__(__FUNCTION__);

#define BEGIN_TIME_BLOCK(NAME) Engine::GetInstance()->GetDebugDisplay().StartFunctionTimer(NAME);

#define END_TIME_BLOCK(NAME) Engine::GetInstance()->GetDebugDisplay().EndFunctionTimer(NAME);


#include "Debug_BlockTimer.h"
#include "GraphRenderer.h"

#include <unordered_map>
#include <bitset>
#include <Vector.h>
#include <sstream>

class Text;
class Camera;
class Font;

class DebugDataDisplay
{
public:
	DebugDataDisplay();
	~DebugDataDisplay();

	void Init();

	void StartFunctionTimer(const std::string& aFunc);
	void EndFunctionTimer(const std::string& aFunc);

	void RecordFrameTime(const float aDeltaTime);
	void Render(Camera& aCamera);

	void ToggleFunctionTimers();
	void ToggleMemoryUsage();
	void ToggleCPUUsage();
	void ToggleFrameTime();

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
	struct FunctionData
	{
		Text* myNameText = nullptr;
		Text* myTimeText = nullptr;
		int myHitCount = 0;
		float myMS = 0.f;
		unsigned long long myStart = 0;
		bool myEnabled = false;
		std::string myNameString;
	};

	struct FrameTimeData
	{
		CU::GrowingArray<float> myFrameTimes;
		float myLastDeltaTime = 0.f;
		float mySampleTimer = 0.f;
		float myTimeBetweenSamples = 0.1f;
		int myFrameTimeIndex = 0;
		int myFrameCounter = 0;
	};

	void RenderFunctionTimers(Camera& aCamera);
	void RenderMemoryUsage(Camera& aCamera);
	void RenderCPUUsage(Camera& aCamera);
	void RenderFrameTime(Camera& aCamera);

	int GetMemoryUsageMB();

	ULONGLONG FixCPUTimings(const FILETIME &a, const FILETIME &b);
	float GetCPUUsage(FILETIME *prevSysKernel, FILETIME *prevSysUser,
		FILETIME *prevProcKernel, FILETIME *prevProcUser,
		bool firstRun = false);

	std::unordered_map<std::string, FunctionData> myFunctionTimers;
	Text* myText;
	unsigned long long myFrequency;
	FILETIME myPrevSysKernel;
	FILETIME myPrevSysUser;
	FILETIME myPrevProcKernel;
	FILETIME myPrevProcUser;

	std::bitset<eBitSetEnum::count> myBoolContainer;

	CU::Vector2<float> myFunctionTimersStartPos;
	CU::Vector2<float> myMemUsageStartPos;
	CU::Vector2<float> myCPUUSageStartPos;
	CU::Vector2<float> myFrameTimeStartPos;
	float myTextScale;

	GraphRenderer myGraphRenderer;
	FrameTimeData myFrameData; //GrowingArray + 16 byte

	std::stringstream myStringStream;
};

inline void DebugDataDisplay::ToggleFunctionTimers()
{
	myBoolContainer[eBitSetEnum::FUNCTION_TIMERS].flip();
}

inline void DebugDataDisplay::ToggleMemoryUsage()
{
	myBoolContainer[eBitSetEnum::MEMORY_USAGE].flip();
}

inline void DebugDataDisplay::ToggleCPUUsage()
{
	myBoolContainer[eBitSetEnum::CPU_USAGE].flip();
}

inline void DebugDataDisplay::ToggleFrameTime()
{
	myBoolContainer[eBitSetEnum::FRAME_TIME].flip();
}

