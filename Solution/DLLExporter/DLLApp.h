#pragma once
#pragma warning(disable : 4005)

#include <InputWrapper.h>
#include <string>
#include <Vector.h>
#include <Windows.h>

class Entity;

namespace Prism
{
	class Camera;
	class Instance;
	class DirectionalLight;
	class Scene;

	struct SetupInfo;
};

namespace CU
{
	class InputWrapper;
}

class DLLApp
{
public:
	DLLApp(int* aHwnd, Prism::SetupInfo& aWindowSetup, WNDPROC aWindowProc);
	~DLLApp();

	void Render();
	void Update();

	void LoadModel(const char* aModelFile, const char* aShaderFile);

	void SetClearColor(CU::Vector4f& aClearColor);

	void SetAutoRotationSpeed(const CU::Vector3f& aRotationSpeed);
	const CU::Vector3f& GetAutoRotationSpeed() const;

	void SetManualRotationAngle(const CU::Vector3f& aRotationAngle);
	const CU::Vector3f& GetManualRotationAngle() const;

	void SetMouseSensitivty(float aValue);
	float GetMouseSensitivty() const;

	CU::Vector3f GetDirectionalLightRotation() const;
	void SetDirectionalLightRotation(CU::Vector3f aRotation);

	void SetCameraZoomSpeed(float aValue);
	float GetCameraZoomSpeed() const;

	void SetCameraMovementSpeed(float aValue);
	float GetCameraMovementSpeed() const;

	void SetCameraRotationSpeed(float aValue);
	float GetCameraRotationSpeed() const;

private:
	void WatchCurrentFiles(const char* aModelFile, const char* aShaderFile);

	void LogicUpdate(float aDeltaTime);

	void AutoRotateObject(float aDeltaTime);
	void ManualRotateObject();

	void CameraRotation(float aDeltaTime);
	void CameraPan(float aDeltaTime);
	void CameraZoom(float aDeltaTime);

	bool HasMouseDeltaXMoved();
	bool HasMouseDeltaYMoved();

	void SetupInput();
	void SetupLight();
	void SetupCamera(Prism::SetupInfo& aWindowSetup);
	void SetupObject();
	void SetupCubeMap();
	void SetParentWindow(Prism::SetupInfo &aWindowSetup);

	CU::InputWrapper myInput;

	Entity* myObjectEntity;
	Entity* myCameraEntity;

	Prism::DirectionalLight* myDirectionalLight;
	Prism::Camera* myCamera;
	Prism::Scene* myScene;

	CU::Vector3f myAutoRotationSpeed;
	CU::Vector3f myManualRotationAngle;
	CU::Vector3f myDirectionalLightRotation;
	
	float myCameraMovementSpeed;
	float myCameraRotationSpeed;
	float myCameraZoomSpeed;
	float myMouseSensitivty;

	HWND myEngineWindowHandler;
	HWND myPanelWindowHandler;

	std::string myModelFile;
	std::string myShaderFile;
};

inline bool DLLApp::HasMouseDeltaYMoved()
{
	return (myInput.GetMouseDY() < myMouseSensitivty || myInput.GetMouseDY() > myMouseSensitivty);
}

inline bool DLLApp::HasMouseDeltaXMoved()
{
	return (myInput.GetMouseDX() < myMouseSensitivty || myInput.GetMouseDX() > myMouseSensitivty);
}

inline void DLLApp::SetAutoRotationSpeed(const CU::Vector3f& aRotationSpeed)
{
	myAutoRotationSpeed = aRotationSpeed;
}

inline const CU::Vector3f& DLLApp::GetAutoRotationSpeed() const
{
	return myAutoRotationSpeed;
}

inline void DLLApp::SetManualRotationAngle(const CU::Vector3f& aRotationAngle)
{
	myManualRotationAngle = aRotationAngle;
	ManualRotateObject();
}
inline const CU::Vector3f& DLLApp::GetManualRotationAngle() const
{
	return myManualRotationAngle;
}

inline float DLLApp::GetMouseSensitivty() const
{
	return myMouseSensitivty;
}
inline void DLLApp::SetMouseSensitivty(float val)
{
	myMouseSensitivty = val;
}

inline CU::Vector3f DLLApp::GetDirectionalLightRotation() const
{
	return myDirectionalLightRotation;
}
inline void DLLApp::SetDirectionalLightRotation(CU::Vector3f aRotation)
{
	myDirectionalLightRotation = aRotation;
}

inline void DLLApp::SetCameraZoomSpeed(float aValue)
{
	myCameraZoomSpeed = aValue;
}

inline float DLLApp::GetCameraZoomSpeed() const
{
	return myCameraZoomSpeed;
}

inline void DLLApp::SetCameraMovementSpeed(float aValue)
{
	myCameraMovementSpeed = aValue;
}

inline float DLLApp::GetCameraMovementSpeed() const
{
	return myCameraMovementSpeed;
}

inline void DLLApp::SetCameraRotationSpeed(float aValue)
{
	myCameraRotationSpeed = aValue;
}

inline float DLLApp::GetCameraRotationSpeed() const
{
	return myCameraRotationSpeed;
}