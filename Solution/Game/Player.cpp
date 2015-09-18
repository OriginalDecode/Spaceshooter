#include "stdafx.h"
#include "Constants.h"
#include "../Engine/Text.h"
#include <InputWrapper.h>
#include "Player.h"
#include <sstream>
#include <XMLReader.h>

// for test shot
#include "Entity.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include <Instance.h>

Player::Player(CU::InputWrapper& aInputWrapper)
	: myInputWrapper(aInputWrapper)
{
	XMLReader reader;
	reader.OpenDocument("Data/script/player.xml");
	reader.ReadAttribute(reader.FindFirstChild("steering"), "modifier", mySteeringModifier);
	myCursorPosition;

	myTestBullet = new Entity();
	myTestBullet->AddComponent<GraphicsComponent>()->InitCube(0.5, 0.5, 0.5);
	myTestBullet->GetComponent<GraphicsComponent>()->SetPosition(myPosition);
	myTestBullet->AddComponent<PhysicsComponent>()->Init({ 0, 0, 0 }, myPosition);
	myIsShooting = false;
}

CU::Matrix44f& Player::GetOrientation() 
{
	return myOrientation;
}

void Player::Update(float aDeltaTime)
{
	if (myInputWrapper.KeyIsPressed(DIK_W) == true)
	{
		MoveForward(50 * aDeltaTime);
	}
	if (myInputWrapper.KeyIsPressed(DIK_S) == true)
	{
		MoveForward(-50 * aDeltaTime);
	}
	if (myInputWrapper.KeyIsPressed(DIK_D) == true)
	{
		MoveRight(50 * aDeltaTime);
	}
	if (myInputWrapper.KeyIsPressed(DIK_A) == true)
	{
		MoveRight(-50 * aDeltaTime);
	}

	if (myInputWrapper.KeyIsPressed(DIK_Q) == true)
	{
		RotateZ(aDeltaTime);
	}
	if (myInputWrapper.KeyIsPressed(DIK_E) == true)
	{
		RotateZ(-aDeltaTime);
	}
	if (myInputWrapper.MouseIsPressed(0) == true)
	{
		//ShootTest();
	}

	myCursorPosition.x += static_cast<float>(myInputWrapper.GetMouseDX()) * 0.001f;
	myCursorPosition.y += static_cast<float>(myInputWrapper.GetMouseDY()) * 0.001f;

	float negateX = myCursorPosition.x > 0.0f ? 1.0f : -1.0f;
	float negateY = myCursorPosition.y > 0.0f ? 1.0f : -1.0f;

	float x = myCursorPosition.x;
	float y = myCursorPosition.y;

	if (x < 0.001f && x > -0.001f)
	{
		x = 0;
	}
	if (y < 0.001f && y > -0.001f)
	{
		y = 0;
	}


	float xRotation = (fabs((x*x) * mySteeringModifier)  * negateX) * aDeltaTime;
	float yRotation = (fabs((y*y) * mySteeringModifier)  * negateY) * aDeltaTime;

	std::stringstream ss;
	ss << "\nCursorPosX: " << xRotation << " CursorPosY: " << yRotation;
	OutputDebugStringA(ss.str().c_str());


	myOrientation = myOrientation.CreateRotateAroundY(xRotation)
		* myOrientation;
	myOrientation = myOrientation.CreateRotateAroundX(yRotation)
		* myOrientation;

	if (myIsShooting == true)
	{
		myTestBullet->GetComponent<PhysicsComponent>()->Update(aDeltaTime);
		myTestBullet->GetComponent<GraphicsComponent>()->SetPosition(myTestBullet->GetComponent<PhysicsComponent>()->GetPosition());
	}
}

void Player::Render(Prism::Camera* aCamera)
{
	if (myIsShooting == true)
	{
		myTestBullet->GetComponent<GraphicsComponent>()->GetInstance()->Render(*aCamera);
	}
}

void Player::MoveForward(float aDistance)
{
	myPosition += myOrientation.GetForward() * aDistance;
	myOrientation.SetPos(myPosition);
}

void Player::MoveRight(float aDistance)
{
	myPosition += myOrientation.GetRight() * aDistance;
	myOrientation.SetPos(myPosition);
}

void Player::RotateX(float aRadian)
{
	myPosition = myOrientation.GetPos();
	myOrientation.SetPos({ 0.f, 0.f, 0.f, 0.f });
	myOrientation = CU::Matrix44<float>::CreateRotateAroundX(aRadian) * myOrientation;
	myOrientation.SetPos(myPosition);
}

void Player::RotateY(float aRadian)
{
	myPosition = myOrientation.GetPos();
	myOrientation.SetPos({ 0.f, 0.f, 0.f, 0.f });
	myOrientation = CU::Matrix44<float>::CreateRotateAroundY(aRadian) * myOrientation;
	myOrientation.SetPos(myPosition);
}

void Player::RotateZ(float aRadian)
{
	myPosition = myOrientation.GetPos();
	myOrientation.SetPos({ 0.f, 0.f, 0.f, 0.f });
	myOrientation = CU::Matrix44<float>::CreateRotateAroundZ(aRadian) * myOrientation;
	myOrientation.SetPos(myPosition);
}

void Player::ShootTest()
{
	myIsShooting = true;
	myTestBullet->GetComponent<PhysicsComponent>()->Init(myOrientation.GetForward() * 50.f, myPosition);
}
