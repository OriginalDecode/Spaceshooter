#include "stdafx.h"

#include "ControllerComponent.h"
#include "Entity.h"
#include "ShootNote.h"
#include "ShootingComponent.h"

ControllerComponent::ControllerComponent(Entity& aEntity)
	: Component(aEntity)
{

}

void ControllerComponent::MoveUp(float aDistance)
{
	myEntity.myOrientation.SetPos(myEntity.myOrientation.GetPos() 
		+ myEntity.myOrientation.GetUp() * aDistance);
}

void ControllerComponent::MoveDown(float aDistance)
{
	myEntity.myOrientation.SetPos(myEntity.myOrientation.GetPos() 
		+ myEntity.myOrientation.GetUp() * (-aDistance));
}

void ControllerComponent::MoveLeft(float aDistance)
{
	myEntity.myOrientation.SetPos(myEntity.myOrientation.GetPos() 
		+ myEntity.myOrientation.GetRight() * (-aDistance));
}

void ControllerComponent::MoveRight(float aDistance)
{
	myEntity.myOrientation.SetPos(myEntity.myOrientation.GetPos() 
		+ myEntity.myOrientation.GetRight() * aDistance);
}

void ControllerComponent::MoveForward(float aDistance)
{
	myEntity.myOrientation.SetPos(myEntity.myOrientation.GetPos() 
		+ myEntity.myOrientation.GetForward() * aDistance);
}

void ControllerComponent::MoveBackward(float aDistance)
{
	myEntity.myOrientation.SetPos(myEntity.myOrientation.GetPos() 
		+ myEntity.myOrientation.GetForward() * (-aDistance));
}

void ControllerComponent::Move(const CU::Vector3<float>& aDirection)
{
	myEntity.myOrientation.SetPos(myEntity.myOrientation.GetPos()
		+ aDirection);
}

void ControllerComponent::RotateX(float aAmount)
{
	myEntity.myOrientation = CU::Matrix44<float>::CreateRotateAroundX(aAmount) * myEntity.myOrientation;
}

void ControllerComponent::RotateY(float aAmount)
{
	myEntity.myOrientation = CU::Matrix44<float>::CreateRotateAroundY(aAmount) * myEntity.myOrientation;
}

void ControllerComponent::RotateZ(float aAmount)
{
	myEntity.myOrientation = CU::Matrix44<float>::CreateRotateAroundZ(aAmount) * myEntity.myOrientation;
}

void ControllerComponent::Rotate(const CU::Matrix44<float>& aRotation)
{
	CU::Vector4<float> pos = myEntity.myOrientation.GetPos();
	myEntity.myOrientation.SetPos({ 0.f, 0.f, 0.f, 1.f });
	myEntity.myOrientation = aRotation * myEntity.myOrientation;
	myEntity.myOrientation.SetPos(pos);
}

void ControllerComponent::SetRotation(const CU::Matrix44<float>& aRotation)
{
	CU::Vector4<float> pos = myEntity.myOrientation.GetPos();
	myEntity.myOrientation = aRotation;
	myEntity.myOrientation.SetPos(pos);
}

void ControllerComponent::Shoot()
{
	DL_ASSERT_EXP(myEntity.GetComponent<ShootingComponent>() != nullptr, "Cant Shoot without ShootingComponent");
	ShootNote note;
	myEntity.SendNote(note);
}