#include "stdafx.h"

#include "DebugDataDisplay.h"
#include "Font.h"
#include <Psapi.h>
#include <sstream>
#include "Text.h"
#include "VTuneApi.h"


Prism::DebugDataDisplay::DebugDataDisplay()
	: myFunctionTimersStartPos(0.f, 0.f)
	, myMemUsageStartPos(1000.f, 0.f)
	, myCPUUSageStartPos(1000.f, -30.f)
	, myFrameTimeStartPos(1000.f, -60.f)
	, myTextScale(0.75f) 
{
}

void Prism::DebugDataDisplay::Init()
{
	myText = new Text();
	myText->Init(Engine::GetInstance()->GetFontContainer().GetFont("Data/resources/font/font.dds"));

	LARGE_INTEGER largeInteger;
	QueryPerformanceFrequency(&largeInteger);
	myFrequency = largeInteger.QuadPart;

	GetCPUUsage(&myPrevSysKernel, &myPrevSysUser, &myPrevProcKernel, &myPrevProcUser, true);

	myGraphRenderer.Init();

	myFrameData.myFrameTimes.Init(60);
	myFrameData.myFrameTimeIndex = 0;
	myFrameData.mySampleTimer = 1.f;
	myFrameData.myFrameCounter = 0;
	myFrameData.myLastDeltaTime = 0.f;
}

void Prism::DebugDataDisplay::StartFunctionTimer(const std::string& aFunc)
{
	FunctionData& it = myFunctionTimers[aFunc];

	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);

	it.myStart = time.QuadPart;
}

void Prism::DebugDataDisplay::EndFunctionTimer(const std::string& aFunc)
{
	auto it = myFunctionTimers.find(aFunc);

	if (it != myFunctionTimers.end())
	{
		if (it->second.myNameText == nullptr)
		{
			//Since the namestring wont ever change for each function I can 
			//create it when we initialize the func-timer, so I dont have to
			//create another stringstream for the name when printing later
			myStringStream.clear();
			myStringStream.str(std::string());
			myStringStream << aFunc << ":" << std::endl;
			it->second.myNameString = myStringStream.str();


			it->second.myNameText = new Text();
			it->second.myNameText->Init(
				Engine::GetInstance()->GetFontContainer().GetFont("Data/resources/font/font.dds"));

			it->second.myTimeText = new Text();
			it->second.myTimeText->Init(
				Engine::GetInstance()->GetFontContainer().GetFont("Data/resources/font/font.dds"));
		}

		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);

		++it->second.myHitCount;
		it->second.myEnabled = true;

		float ms = static_cast<float>(
			((time.QuadPart * 1000000 / myFrequency) - (it->second.myStart * 1000000 / myFrequency)) / 1000.f);
		it->second.myMS = ms;
	}
}

void Prism::DebugDataDisplay::RecordFrameTime(const float aDeltaTime)
{
	myBoolContainer.set(eBitSetEnum::NEW_GRAPH_DATA, false);

	myFrameData.myLastDeltaTime = aDeltaTime;
	myFrameData.mySampleTimer += aDeltaTime;
	++myFrameData.myFrameCounter;

	if (myFrameData.mySampleTimer > myFrameData.myTimeBetweenSamples)
	{
		myBoolContainer.set(eBitSetEnum::NEW_GRAPH_DATA, true);

		float frameTime = myFrameData.mySampleTimer / myFrameData.myFrameCounter;
		myFrameData.mySampleTimer = 0.f;
		myFrameData.myFrameCounter = 0;

		if (myFrameData.myFrameTimes.Size() < 60)
		{
			myFrameData.myFrameTimes.Add(frameTime * 1000.f);
			myFrameData.myFrameTimeIndex;
		}
		else
		{
			if (myFrameData.myFrameTimeIndex >= myFrameData.myFrameTimes.Size())
				myFrameData.myFrameTimeIndex = 0;

			myFrameData.myFrameTimes[myFrameData.myFrameTimeIndex] = frameTime * 1000.f;
			++myFrameData.myFrameTimeIndex;
		}
	}
}

void Prism::DebugDataDisplay::Render(const Camera& aCamera)
{
	if (myBoolContainer.at(eBitSetEnum::FUNCTION_TIMERS))
	{
		RenderFunctionTimers(aCamera);
	}

	if (myBoolContainer.at(eBitSetEnum::MEMORY_USAGE))
	{
		RenderMemoryUsage(aCamera);
	}

	if (myBoolContainer.at(eBitSetEnum::CPU_USAGE))
	{
		RenderCPUUsage(aCamera);
	}

	if (myBoolContainer.at(eBitSetEnum::FRAME_TIME))
	{
		RenderFrameTime(aCamera);
	}
}

void Prism::DebugDataDisplay::RenderFunctionTimers(const Camera& aCamera)
{
	VTUNE_EVENT_BEGIN(VTUNE::FUNCTION_TIMERS);

	TIME_FUNCTION

	CU::Vector2<float> drawPos = myFunctionTimersStartPos;
	for (auto it = myFunctionTimers.begin(); it != myFunctionTimers.end(); ++it)
	{
		if (it->second.myEnabled == true)
		{
			myStringStream.clear();
			myStringStream.str(std::string());
			myStringStream << it->second.myMS << " ms";
			if (it->second.myHitCount > 1)
				myStringStream << " (" << it->second.myHitCount << " calls)";

			myStringStream << std::endl;

			it->second.myNameText->UpdateSentence(it->second.myNameString.c_str(), drawPos.x, drawPos.y, myTextScale);
			it->second.myNameText->Render(aCamera);

			it->second.myTimeText->UpdateSentence(myStringStream.str().c_str()
				, drawPos.x + it->second.myNameText->GetTextWidth() + 10.f, drawPos.y, myTextScale);
			it->second.myTimeText->Render(aCamera);
			drawPos.y -= 30.f;

			//FUNCTION_TIMER_LOG("%s took %s", it->second.myNameString.c_str(), myStringStream.str().c_str());

			it->second.myEnabled = false;
			it->second.myHitCount = 0;
		}
	}

	VTUNE_EVENT_END();
}

void Prism::DebugDataDisplay::RenderMemoryUsage(const Camera& aCamera)
{
	TIME_FUNCTION

	myStringStream.clear();
	myStringStream.str(std::string());
	myStringStream << "Memory: " << GetMemoryUsageMB() << "mb" << std::endl;

	myText->UpdateSentence(myStringStream.str().c_str(), myMemUsageStartPos.x, myMemUsageStartPos.y, myTextScale);
	myText->Render(aCamera);
}

void Prism::DebugDataDisplay::RenderCPUUsage(const Camera& aCamera)
{
	TIME_FUNCTION

	myStringStream.clear();
	myStringStream.str(std::string());
	myStringStream << "CPU: " 
		<< GetCPUUsage(&myPrevSysKernel, &myPrevSysUser, &myPrevProcKernel, &myPrevProcUser) << "%" << std::endl;

	myText->UpdateSentence(myStringStream.str().c_str(), myCPUUSageStartPos.x, myCPUUSageStartPos.y, myTextScale);
	myText->Render(aCamera);
}

void Prism::DebugDataDisplay::RenderFrameTime(const Camera& aCamera)
{
	TIME_FUNCTION

	float FPS = 1.f / myFrameData.myLastDeltaTime;
	myStringStream.clear();
	myStringStream.str(std::string());
	myStringStream << "FPS: " << FPS << std::endl;

	myText->UpdateSentence(myStringStream.str().c_str(), myFrameTimeStartPos.x, myFrameTimeStartPos.y, myTextScale);
	myText->Render(aCamera);


	float frameTimeMS = myFrameData.myLastDeltaTime * 1000.f;
	myStringStream.clear();
	myStringStream.str(std::string());
	myStringStream << "FrameTime: " << frameTimeMS << "ms" << std::endl;

	myText->UpdateSentence(myStringStream.str().c_str(), myFrameTimeStartPos.x, myFrameTimeStartPos.y - 30.f, myTextScale);
	myText->Render(aCamera);

	float graphHeight = 50.f;
	float graphWidth = 200.f;
	myGraphRenderer.Render(aCamera, myFrameData.myFrameTimes
		, { myFrameTimeStartPos.x - graphWidth - 40.f, myFrameTimeStartPos.y - 30.f }
		, { graphWidth, graphHeight }, 16.f, myBoolContainer.at(eBitSetEnum::NEW_GRAPH_DATA));
}


int Prism::DebugDataDisplay::GetMemoryUsageMB()
{
	PROCESS_MEMORY_COUNTERS memCounter;
	GetProcessMemoryInfo(GetCurrentProcess(), &memCounter, sizeof(memCounter));

	//int memUsed = memCounter.WorkingSetSize / 1024;
	int memUsedMb = memCounter.WorkingSetSize / 1024 / 1024;

	return memUsedMb;
}

ULONGLONG Prism::DebugDataDisplay::FixCPUTimings(const FILETIME &a, const FILETIME &b)
{
	LARGE_INTEGER la, lb;
	la.LowPart = a.dwLowDateTime;
	la.HighPart = a.dwHighDateTime;
	lb.LowPart = b.dwLowDateTime;
	lb.HighPart = b.dwHighDateTime;

	return la.QuadPart - lb.QuadPart;
}

float Prism::DebugDataDisplay::GetCPUUsage(FILETIME *prevSysKernel, FILETIME *prevSysUser,
	FILETIME *prevProcKernel, FILETIME *prevProcUser,
	bool firstRun)
{
	FILETIME sysIdle, sysKernel, sysUser;
	FILETIME procCreation, procExit, procKernel, procUser;

	if (!GetSystemTimes(&sysIdle, &sysKernel, &sysUser) ||
		!GetProcessTimes(GetCurrentProcess(), &procCreation, &procExit, &procKernel, &procUser))
	{
		// can't get time info so return
		return -1.;
	}

	// check for first call
	if (firstRun)
	{
		// save time info before return
		prevSysKernel->dwLowDateTime = sysKernel.dwLowDateTime;
		prevSysKernel->dwHighDateTime = sysKernel.dwHighDateTime;

		prevSysUser->dwLowDateTime = sysUser.dwLowDateTime;
		prevSysUser->dwHighDateTime = sysUser.dwHighDateTime;

		prevProcKernel->dwLowDateTime = procKernel.dwLowDateTime;
		prevProcKernel->dwHighDateTime = procKernel.dwHighDateTime;

		prevProcUser->dwLowDateTime = procUser.dwLowDateTime;
		prevProcUser->dwHighDateTime = procUser.dwHighDateTime;

		return -1.;
	}

	ULONGLONG sysKernelDiff = FixCPUTimings(sysKernel, *prevSysKernel);
	ULONGLONG sysUserDiff = FixCPUTimings(sysUser, *prevSysUser);

	ULONGLONG procKernelDiff = FixCPUTimings(procKernel, *prevProcKernel);
	ULONGLONG procUserDiff = FixCPUTimings(procUser, *prevProcUser);

	ULONGLONG sysTotal = sysKernelDiff + sysUserDiff;
	ULONGLONG procTotal = procKernelDiff + procUserDiff;

	return (float)((100.0 * procTotal) / sysTotal);
}