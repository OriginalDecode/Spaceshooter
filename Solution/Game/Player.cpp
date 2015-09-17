#include "stdafx.h"
#include "Constants.h"
#include <InputWrapper.h>
#include "Player.h"


Player::Player(CU::InputWrapper& aInputWrapper)
	: myInputWrapper(aInputWrapper)
{

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
		MoveForward(50 * aDeltaTime);
	}
	if (myInputWrapper.KeyIsPressed(DIK_A) == true)
	{
		MoveRight(-50 * aDeltaTime);
	}

	if (myInputWrapper.KeyIsPressed(DIK_E) == true)
	{
		RotateZ(aDeltaTime);
	}
	if (myInputWrapper.KeyIsPressed(DIK_Q) == true)
	{
		RotateZ(-aDeltaTime);
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
