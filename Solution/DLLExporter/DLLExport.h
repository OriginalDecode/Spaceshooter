#pragma once

#define DLLExport __declspec(dllexport)

extern "C" DLLExport void StartEngine(int* aHwnd);
extern "C" DLLExport void SetupWindow(int aWidth, int aHeight);
extern "C" DLLExport void Render();
extern "C" DLLExport void Update();
extern "C" DLLExport void LoadModel(const char* aModelFile, const char* aEffectFile);
extern "C" DLLExport void SetEffect(const char* aEffectFile);
extern "C" DLLExport void SetClearColor(float aRChannel, float aGChannel, float aBChannel, float aAChannel);