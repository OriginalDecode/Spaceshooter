#include "stdafx.h"

#include "ControllerComponent.h"
#include "Entity.h"
#include "ShootMessage.h"
#include "RefreshOrientationMessage.h"

void ControllerComponent::MoveUp(float aDistance)
{
	myEntity->myOrientation.SetPos(myEntity->myOrientation.GetPos() 
		+ myEntity->myOrientation.GetUp() * aDistance);
	myEntity->SendMessage(RefreshOrientationMessage());
}

void ControllerComponent::MoveDown(float aDistance)
{
	myEntity->myOrientation.SetPos(myEntity->myOrientation.GetPos() 
		+ myEntity->myOrientation.GetUp() * (-aDistance));
	myEntity->SendMessage(RefreshOrientationMessage());
}

void ControllerComponent::MoveLeft(float aDistance)
{
	myEntity->myOrientation.SetPos(myEntity->myOrientation.GetPos() 
		+ myEntity->myOrientation.GetRight() * (-aDistance));
	myEntity->SendMessage(RefreshOrientationMessage());
}

void ControllerComponent::MoveRight(float aDistance)
{
	myEntity->myOrientation.SetPos(myEntity->myOrientation.GetPos() 
		+ myEntity->myOrientation.GetRight() * aDistance);
	myEntity->SendMessage(RefreshOrientationMessage());
}

void ControllerComponent::MoveForward(float aDistance)
{
	myEntity->myOrientation.SetPos(myEntity->myOrientation.GetPos() 
		+ myEntity->myOrientation.GetForward() * aDistance);
	myEntity->SendMessage(RefreshOrientationMessage());
}

void ControllerComponent::MoveBackward(float aDistance)
{
	myEntity->myOrientation.SetPos(myEntity->myOrientation.GetPos() 
		+ myEntity->myOrientation.GetForward() * (-aDistance));
	myEntity->SendMessage(RefreshOrientationMessage());
}

void ControllerComponent::RotateX(float aAmount)
{
	myEntity->myOrientation = CU::Matrix44<float>::CreateRotateAroundX(aAmount) * myEntity->myOrientation;
	myEntity->SendMessage(RefreshOrientationMessage());
}

void ControllerComponent::RotateY(float aAmount)
{
	myEntity->myOrientation = CU::Matrix44<float>::CreateRotateAroundY(aAmount) * myEntity->myOrientation;
	myEntity->SendMessage(RefreshOrientationMessage());
}

void ControllerComponent::RotateZ(float aAmount)
{
	myEntity->myOrientation = CU::Matrix44<float>::CreateRotateAroundZ(aAmount) * myEntity->myOrientation;
	myEntity->SendMessage(RefreshOrientationMessage());
}

void ControllerComponent::Rotate(const CU::Matrix44<float>& aRotation)
{
	CU::Vector4<float> pos = myEntity->myOrientation.GetPos();
	myEntity->myOrientation.SetPos({ 0.f, 0.f, 0.f, 1.f });
	myEntity->myOrientation = aRotation * myEntity->myOrientation;
	myEntity->myOrientation.SetPos(pos);
	myEntity->SendMessage(RefreshOrientationMessage());
}

void ControllerComponent::Shoot(float aSpeed)
{
	ShootMessage msg(aSpeed);
	myEntity->SendMessage(msg);
}