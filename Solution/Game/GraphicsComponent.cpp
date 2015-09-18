#include "stdafx.h"

#include "GraphicsComponent.h"
#include <Engine.h>
#include <Instance.h>
#include <Model.h>
#include "TranslationMessage.h"

GraphicsComponent::GraphicsComponent()
	: myInstance(nullptr)
{
}

void GraphicsComponent::Init(const char* aModelPath, const char* aEffectPath)
{
	Prism::Model* model = Prism::Engine::GetInstance()->LoadModel(aModelPath
		, Prism::Engine::GetInstance()->GetEffectContainer().GetEffect(aEffectPath));

	myInstance = new Prism::Instance(*model);
}

void GraphicsComponent::InitGeometry(const Prism::MeshData& aMeshData)
{
	Prism::Model* model = new Prism::Model();
	model->InitGeometry(aMeshData);

	myInstance = new Prism::Instance(*model);
}

void GraphicsComponent::InitCube(float aWidth, float aHeight, float aDepth)
{
	Prism::Model* model = new Prism::Model();
	model->InitCube(aWidth, aHeight, aDepth);

	myInstance = new Prism::Instance(*model);
}

void GraphicsComponent::Update(float aDeltaTime)
{
	aDeltaTime;
}

void GraphicsComponent::ReceiveMessage(const TranslationMessage& aMessage)
{
	CU::Vector3<float> pos = myInstance->GetPosition();
	CU::Vector3<float> forward = myInstance->GetOrientation().GetForward();
	CU::Vector3<float> right = myInstance->GetOrientation().GetRight();
	CU::Vector3<float> up = myInstance->GetOrientation().GetUp();

	switch (aMessage.GetTranslationType())
	{
	case eTranslationType::MOVE_UP:
		pos += up * aMessage.GetTranslationAmount();
		break;
	case eTranslationType::MOVE_DOWN:
		pos += up * (-aMessage.GetTranslationAmount());
		break;
	case eTranslationType::MOVE_LEFT:
		pos += right * (-aMessage.GetTranslationAmount());
		break;
	case eTranslationType::MOVE_RIGHT:
		pos += right * aMessage.GetTranslationAmount();
		break;
	case eTranslationType::MOVE_FORWARD:
		pos += forward * aMessage.GetTranslationAmount();
		break;
	case eTranslationType::MOVE_BACKWARD:
		pos += forward * (-aMessage.GetTranslationAmount());
		break;
	case eTranslationType::ROTATE_X:
		myInstance->SetOrientation(CU::Matrix44<float>::CreateRotateAroundX(
			aMessage.GetTranslationAmount()) * myInstance->GetOrientation());
		break;
	case eTranslationType::ROTATE_Y:
		myInstance->SetOrientation(CU::Matrix44<float>::CreateRotateAroundY(
			aMessage.GetTranslationAmount()) * myInstance->GetOrientation());
		break;
	case eTranslationType::ROTATE_Z:
		myInstance->SetOrientation(CU::Matrix44<float>::CreateRotateAroundZ(
			aMessage.GetTranslationAmount()) * myInstance->GetOrientation());
		break;
	default:
		break;
	}

	myInstance->SetPosition(pos);
}

void GraphicsComponent::SetPosition(const CU::Vector3<float>& aPosition)
{
	myInstance->SetPosition(aPosition);
}