#include "stdafx.h"
#include "Constants.h"
#include "../Engine/Text.h"
#include <InputWrapper.h>
#include "Player.h"


Player::Player(CU::InputWrapper& aInputWrapper)
	: myInputWrapper(aInputWrapper)
{
	myCursorPosition;
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

	myCursorPosition.x += myInputWrapper.GetMouseDX() * 0.001f;
	myCursorPosition.y += myInputWrapper.GetMouseDY() * 0.001f;

	float negateX = myCursorPosition.x > 0.0f ? 1.0f : -1.0f;
	float negateY = myCursorPosition.y > 0.0f ? 1.0f : -1.0f;

	float x = myCursorPosition.x;
	float y = myCursorPosition.y;

	myOrientation = myOrientation.CreateRotateAroundY((fabs(x + (pow((x), 4)))  * negateX) * aDeltaTime)
		* myOrientation;
	myOrientation = myOrientation.CreateRotateAroundX((fabs(y + (pow((y), 4)))  * negateY) * aDeltaTime)
		* myOrientation;

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
