#include "stdafx.h"
#include "Camera.h"
#include <d3dx11effect.h>
#include "ParticleEmitterInstance.h"
#include <MathHelper.h>
#include "VertexBufferWrapper.h"
namespace Prism
{
	ParticleEmitterInstance::ParticleEmitterInstance()
		: myVertexWrapper(nullptr)
		, myIsActive(false)
		, myEmissionTime(0)
		, myEmitterLife(0)
		, myParticleIndex(0)
		, myDeadParticleCount(0)
	{
	}

	ParticleEmitterInstance::~ParticleEmitterInstance()
	{
		delete myVertexWrapper;
		myVertexWrapper = nullptr;
	}

	void ParticleEmitterInstance::Initiate(ParticleEmitterData someData)
	{
		myParticleEmitterData = someData;

		int particleCount = static_cast<int>(myParticleEmitterData.myParticlesLifeTime / myParticleEmitterData.myEmissionRate) + 1;

		myGraphicalParticles.Init(particleCount);
		myLogicalParticles.Init(particleCount);

		myEmissionTime = myParticleEmitterData.myEmissionRate;

		myDiffColor = (myParticleEmitterData.myData.myEndColor - myParticleEmitterData.myData.myStartColor) / myParticleEmitterData.myParticlesLifeTime;

		for (int i = 0; i < particleCount; ++i)
		{
			GraphicalParticle tempGraphics;
			myGraphicalParticles.Add(tempGraphics);
			LogicalParticle tempLogic;
			myLogicalParticles.Add(tempLogic);
		}

		myIsActive = myParticleEmitterData.myIsActiveAtStart;

		myEmitterLife = myParticleEmitterData.myEmitterLifeTime;

		CreateVertexBuffer();
	}

	void ParticleEmitterInstance::Render(Camera* aCamera)
	{
		UpdateVertexBuffer();

		myParticleEmitterData.myEffect->SetTexture(
			Engine::GetInstance()->GetTextureContainer()->GetTexture(myParticleEmitterData.myTextureName.c_str()));


		myParticleEmitterData.myEffect->SetWorldMatrix(myOrientation);
		myParticleEmitterData.myEffect->SetViewMatrix(CU::InverseSimple(aCamera->GetOrientation()));
		myParticleEmitterData.myEffect->SetProjectionMatrix(aCamera->GetProjection());

		Engine::GetInstance()->GetContex()->IASetInputLayout(myParticleEmitterData.myInputLayout);
		Engine::GetInstance()->GetContex()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		Engine::GetInstance()->GetContex()->IASetVertexBuffers(
			myVertexWrapper->myStartSlot
			, myVertexWrapper->myNumberOfBuffers
			, &myVertexWrapper->myVertexBuffer
			, &myVertexWrapper->myStride
			, &myVertexWrapper->myByteOffset);

		D3DX11_TECHNIQUE_DESC techDesc;
		myParticleEmitterData.myEffect->GetTechnique()->GetDesc(&techDesc);

		for (UINT i = 0; i < techDesc.Passes; ++i)
		{
			myParticleEmitterData.myEffect->GetTechnique()->GetPassByIndex(i)->Apply(0, Engine::GetInstance()->GetContex());
			Engine::GetInstance()->GetContex()->Draw(myGraphicalParticles.Size(), 0);
		}
	}

	void ParticleEmitterInstance::Update(float aDeltaTime, const CU::Matrix44f& aWorldMatrix)
	{
		UpdateEmitter(aDeltaTime,aWorldMatrix);
	}

	bool ParticleEmitterInstance::GetIsActive()
	{
		return myIsActive;
	}

	void ParticleEmitterInstance::ToggleActive()
	{
		myIsActive = !myIsActive;
		myEmitterLife = myParticleEmitterData.myEmitterLifeTime;
		myDeadParticleCount = 0;
	}

	void ParticleEmitterInstance::ToggleActive(bool aIsActive)
	{
		myIsActive = aIsActive;
		myEmitterLife = myParticleEmitterData.myEmitterLifeTime;
		myDeadParticleCount = 0;
	}

	void ParticleEmitterInstance::CreateVertexBuffer()
	{

		myVertexWrapper = new VertexBufferWrapper();
		myVertexWrapper->myStride = sizeof(GraphicalParticle);
		myVertexWrapper->myByteOffset = 0;
		myVertexWrapper->myStartSlot = 0;
		myVertexWrapper->myNumberOfBuffers = 1;

		HRESULT hr;
		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		if (myVertexWrapper->myVertexBuffer != nullptr)
		{
			myVertexWrapper->myVertexBuffer->Release();
		}

		vertexBufferDesc.ByteWidth = sizeof(GraphicalParticle) * myGraphicalParticles.Size();

		hr = Engine::GetInstance()->GetDevice()->CreateBuffer(&vertexBufferDesc, nullptr, &myVertexWrapper->myVertexBuffer);
		DL_ASSERT_EXP(hr == S_OK, "[ParticleEmitterInstance](CreateVertexBuffer) : Failed to create VertexBuffer");

	}

	void ParticleEmitterInstance::UpdateVertexBuffer()
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		Engine::GetInstance()->GetContex()->Map(myVertexWrapper->myVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		if (mappedResource.pData != nullptr)
		{
			GraphicalParticle *data = (GraphicalParticle*)mappedResource.pData;

			bool isSafe = sizeof(GraphicalParticle) == sizeof(myGraphicalParticles[0]);
			DL_ASSERT_EXP(isSafe, "[ParticleEmitter](UpdateVertexBuffer) : Not safe to copy.");
			memcpy(data, &myGraphicalParticles[0], sizeof(GraphicalParticle)* myGraphicalParticles.Size());
		}
		Engine::GetInstance()->GetContex()->Unmap(myVertexWrapper->myVertexBuffer, 0);
	}

	void ParticleEmitterInstance::UpdateEmitter(float aDeltaTime, const CU::Matrix44f& aWorldMatrix)
	{
		myEmissionTime -= aDeltaTime;
		myEmitterLife -= aDeltaTime;

		UpdateParticle(aDeltaTime);

		if (myEmissionTime <= 0.f && (myEmitterLife > 0.f || myParticleEmitterData.myUseEmitterLifeTime == false))
		{
			EmitParticle(aWorldMatrix);
			myEmissionTime = myParticleEmitterData.myEmissionRate;
		}
		
		if (myEmitterLife <= 0.f && myDeadParticleCount == myLogicalParticles.Size())
		{
			myIsActive = false;
		}

	}

	void ParticleEmitterInstance::UpdateParticle(float aDeltaTime)
	{
		for (int i = 0; i < myLogicalParticles.Size(); ++i)
		{
			if (myGraphicalParticles[i].myAlpha <= 0.0f)
			{
				myDeadParticleCount++;
				myLogicalParticles[i].myIsAlive = false;
				continue;
			}
			
			myGraphicalParticles[i].myPosition = myGraphicalParticles[i].myPosition -
				(myLogicalParticles[i].myDirection * myLogicalParticles[i].myVelocity) * aDeltaTime;

			myGraphicalParticles[i].myAlpha += myParticleEmitterData.myData.myAlphaDelta * aDeltaTime; 
			myGraphicalParticles[i].mySize += myParticleEmitterData.myData.mySizeDelta * aDeltaTime;

			myGraphicalParticles[i].myColor += myDiffColor * aDeltaTime;

			myGraphicalParticles[i].myLifeTime -= aDeltaTime;

			
		}
	}

	void ParticleEmitterInstance::EmitParticle(const CU::Matrix44f& aWorldMatrix)
	{
		for (int i = 0; i < myParticleEmitterData.myParticlesPerEmitt; ++i)
		{
			if (myParticleIndex == myLogicalParticles.Size() - 1)
			{
				myParticleIndex = 0;
			}
			myDeadParticleCount--;
			myLogicalParticles[myParticleIndex].myIsAlive = true;

			myLogicalParticles[myParticleIndex].myDirection = myParticleEmitterData.myDirection;
			myGraphicalParticles[myParticleIndex].myColor = myParticleEmitterData.myData.myStartColor;

			myGraphicalParticles[myParticleIndex].myPosition =
				CU::Math::RandomRange(aWorldMatrix.GetPos() + myParticleEmitterData.myEmitterSize
				, aWorldMatrix.GetPos() - myParticleEmitterData.myEmitterSize);

			myGraphicalParticles[myParticleIndex].myLifeTime = myParticleEmitterData.myParticlesLifeTime;

			myParticleScaling = CU::Math::RandomRange(myParticleEmitterData.myData.myMinStartSize
				, myParticleEmitterData.myData.myMaxStartSize);

			myGraphicalParticles[myParticleIndex].mySize = 1 * myParticleScaling;

			myLogicalParticles[myParticleIndex].myVelocity.x = CU::Math::RandomRange(myParticleEmitterData.myData.myMinSpeed.x,
				myParticleEmitterData.myData.myMaxSpeed.x);

			myLogicalParticles[myParticleIndex].myVelocity.y = CU::Math::RandomRange(myParticleEmitterData.myData.myMinSpeed.y,
				myParticleEmitterData.myData.myMaxSpeed.y);

			myLogicalParticles[myParticleIndex].myVelocity.z = CU::Math::RandomRange(myParticleEmitterData.myData.myMinSpeed.z,
				myParticleEmitterData.myData.myMaxSpeed.z);

			myGraphicalParticles[myParticleIndex].myAlpha = myParticleEmitterData.myData.myStartAlpha;

			myParticleIndex += 1;
		}

	}
}