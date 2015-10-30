#pragma warning(disable : 4005)

#include "DLLApp.h"
#include "DLLExport.h"
#include <Engine.h>
#include <FileWatcher.h>
#include <SetupInfo.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

DLLApp* locDLLApplication;
Prism::SetupInfo locWindowSetup;

void StartEngine(int* aHwnd)
{
	locDLLApplication = new DLLApp(aHwnd, locWindowSetup, WndProc);
}

void SetupWindow(int aWidth, int aHeight)
{
	locWindowSetup.myMSAACount = 4;
	locWindowSetup.myScreenHeight = aHeight;
	locWindowSetup.myScreenWidth = aWidth;
	locWindowSetup.myWindowed = true;
}

void Render()
{
	locDLLApplication->Render();
}

void Update()
{
	locDLLApplication->Update();
}

void UpdateFilewatcher()
{
	Prism::Engine::GetInstance()->GetFileWatcher()->CheckFiles();
}

void RotateObjectAtX(float aSpeed)
{
	CU::Vector3f rotationSpeed(locDLLApplication->GetAutoRotationSpeed());
	rotationSpeed.myX = aSpeed;
	locDLLApplication->SetAutoRotationSpeed(rotationSpeed);
}

void RotateObjectAtY(float aSpeed)
{
	CU::Vector3f rotationSpeed(locDLLApplication->GetAutoRotationSpeed());
	rotationSpeed.myY = aSpeed;
	locDLLApplication->SetAutoRotationSpeed(rotationSpeed);
}

void RotateObjectAtZ(float aSpeed)
{
	CU::Vector3f rotationSpeed(locDLLApplication->GetAutoRotationSpeed());
	rotationSpeed.myZ = aSpeed;
	locDLLApplication->SetAutoRotationSpeed(rotationSpeed);
}

void SetRotateObjectAtX(float aAngle)
{
	CU::Vector3f rotationAngle(locDLLApplication->GetManualRotationAngle());
	rotationAngle.myX = aAngle;
	locDLLApplication->SetManualRotationAngle(rotationAngle);
}

void SetRotateObjectAtY(float aAngle)
{
	CU::Vector3f rotationAngle(locDLLApplication->GetManualRotationAngle());
	rotationAngle.myY = aAngle;
	locDLLApplication->SetManualRotationAngle(rotationAngle);
}

void SetRotateObjectAtZ(float aAngle)
{
	CU::Vector3f rotationAngle(locDLLApplication->GetManualRotationAngle());
	rotationAngle.myZ = aAngle;
	locDLLApplication->SetManualRotationAngle(rotationAngle);
}

void SetMouseSensitivty(float aValue)
{
	locDLLApplication->SetMouseSensitivty(aValue);
}

float GetMouseSensitivty()
{
	return locDLLApplication->GetMouseSensitivty();
}

void SetCameraZoomSpeed(float aValue)
{
	locDLLApplication->SetCameraZoomSpeed(aValue);
}

float GetCameraZoomSpeed()
{
	return locDLLApplication->GetCameraZoomSpeed();
}

void SetCameraMovementSpeed(float aValue)
{
	locDLLApplication->SetCameraMovementSpeed(aValue);
}

float GetCameraMovementSpeed()
{
	return locDLLApplication->GetCameraMovementSpeed();
}

void SetCameraRotationSpeed(float aValue)
{
	locDLLApplication->SetCameraRotationSpeed(aValue);
}

float GetCameraRotationSpeed()
{
	return locDLLApplication->GetCameraRotationSpeed();
}

void DirectionaLightRotateX(float aAngle)
{
	CU::Vector3<float> rotatedDirection(locDLLApplication->GetDirectionalLightRotation());
	rotatedDirection.myX = aAngle;
	locDLLApplication->SetDirectionalLightRotation(rotatedDirection);
}

void DirectionaLightRotateY(float aAngle)
{
	CU::Vector3<float> rotatedDirection(locDLLApplication->GetDirectionalLightRotation());
	rotatedDirection.myY = aAngle;
	locDLLApplication->SetDirectionalLightRotation(rotatedDirection);
}

void DirectionaLightRotateZ(float aAngle)
{
	CU::Vector3<float> rotatedDirection(locDLLApplication->GetDirectionalLightRotation());
	rotatedDirection.myZ = aAngle;
	locDLLApplication->SetDirectionalLightRotation(rotatedDirection);
}

float GetDirectionaLightXRotation()
{
	return locDLLApplication->GetDirectionalLightRotation().myX;
}

float GetDirectionaLightYRotation()
{
	return locDLLApplication->GetDirectionalLightRotation().myY;
}

float GetDirectionaLightZRotation()
{
	return locDLLApplication->GetDirectionalLightRotation().myZ;
}

void LoadModel(const char* aModelFile, const char* aShaderFile)
{
	locDLLApplication->LoadModel(aModelFile, aShaderFile);
}

void SetClearColor(float aRChannel, float aGChannel, float aBChannel, float aAChannel)
{
	locDLLApplication->SetClearColor(CU::Vector4f(aRChannel, aGChannel, aBChannel, aAChannel));
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}