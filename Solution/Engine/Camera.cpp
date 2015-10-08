#include "stdafx.h"
#include "Camera.h"
#include "FileWatcher.h"
#include "Frustum.h"
#include <xnamath.h>
#include <XMLReader.h>

namespace Prism
{
	Camera::Camera(CU::Matrix44f& aPlayerMatrix)
		: myOrientation(aPlayerMatrix)
		, myNear(0.1f)
		, myFar(2500.f)
	{
		myFrustum = new Frustum(aPlayerMatrix, myNear, myFar);
		WATCH_FILE("Data/script/camera.xml", Camera::ReadXML);
		ReadXML("Data/script/camera.xml");
	}


	Camera::~Camera()
	{
		delete myFrustum;
	}

	void Camera::ReadXML(const std::string& aFileName)
	{
		Sleep(10);
		XMLReader reader;
		reader.OpenDocument("Data/script/camera.xml");
		tinyxml2::XMLElement* levelElement = reader.ForceFindFirstChild("camera");
		reader.ForceReadAttribute(levelElement, "fov", myFOV);
		myFOV *= 3.14159f / 180.f;
		OnResize(Engine::GetInstance()->GetWindowSize().x, Engine::GetInstance()->GetWindowSize().y);
	}

	void Camera::OnResize(const int aWidth, const int aHeight)
	{
		myProjectionMatrix = CU::Matrix44<float>::CreateProjectionMatrixLH(myNear, myFar, static_cast<float>(aHeight) / static_cast<float>(aWidth), myFOV);
		myOrthogonalMatrix = CU::Matrix44<float>::CreateOrthogonalMatrixLH(static_cast<float>(aWidth), static_cast<float>(aHeight), 0.1f, 1000.f);



		//XMMATRIX projection = XMMatrixPerspectiveFovLH(myFOV, static_cast<float>(aWidth) / aHeight, myNear, myFar);
		//XMFLOAT4X4 proj;
		//XMStoreFloat4x4(&proj, projection);
		
		//myProjectionMatrix.Init(reinterpret_cast<float*>(proj.m));
	}

	const CU::Matrix44<float>& Camera::GetOrientation() const
	{
		return myOrientation;
	}

	const CU::Matrix44<float>& Camera::GetProjection() const
	{
		return myProjectionMatrix;
	}

	const CU::Matrix44<float>& Camera::GetOrthogonal() const
	{
		return myOrthogonalMatrix;
	}

	void Camera::Update()
	{
		myFrustum->Update();
	}
}