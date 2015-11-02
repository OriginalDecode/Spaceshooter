#include "stdafx.h"
#include "Camera.h"
#include <d3dx11effect.h>
#include "EmitterInstance.h"
#include <MathHelper.h>
#include "VertexBufferWrapper.h"
namespace Prism
{
	EmitterInstance::EmitterInstance()
		: myVertexWrapper(nullptr)
		, myIsActive(false)
		, myEmissionTime(0)
		, myEmitterLife(0)
		, myParticleIndex(0)
		, myDeadParticleCount(0)
	{
	}

	EmitterInstance::~EmitterInstance()
	{
		delete myVertexWrapper;
		myVertexWrapper = nullptr;
	}

	void EmitterInstance::Initiate(EmitterData someData)
	{
		myEmitterData = someData;

		int particleCount = static_cast<int>(myEmitterData.myParticlesLifeTime / myEmitterData.myEmissionRate) + 1;

		myGraphicalParticles.Init(particleCount);
		myLogicalParticles.Init(particleCount);

		myEmissionTime = myEmitterData.myEmissionRate;

		myDiffColor = (myEmitterData.myData.myEndColor - myEmitterData.myData.myStartColor) / myEmitterData.myParticlesLifeTime;

		for (int i = 0; i < particleCount; ++i)
		{
			GraphicalParticle tempGraphics;
			myGraphicalParticles.Add(tempGraphics);
			LogicalParticle tempLogic;
			myLogicalParticles.Add(tempLogic);
		}

		myIsActive = myEmitterData.myIsActiveAtStart;

		myEmitterLife = myEmitterData.myEmitterLifeTime;

		CreateVertexBuffer();
	}

	void EmitterInstance::Render(Camera* aCamera)
	{
		UpdateVertexBuffer();

		myEmitterData.myEffect->SetTexture(
			Engine::GetInstance()->GetTextureContainer()->GetTexture(myEmitterData.myTextureName.c_str()));


		myEmitterData.myEffect->SetWorldMatrix(myOrientation);
		myEmitterData.myEffect->SetViewMatrix(CU::InverseSimple(aCamera->GetOrientation()));
		myEmitterData.myEffect->SetProjectionMatrix(aCamera->GetProjection());

		Engine::GetInstance()->GetContex()->IASetInputLayout(myEmitterData.myInputLayout);
		Engine::GetInstance()->GetContex()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		Engine::GetInstance()->GetContex()->IASetVertexBuffers(
			myVertexWrapper->myStartSlot
			, myVertexWrapper->myNumberOfBuffers
			, &myVertexWrapper->myVertexBuffer
			, &myVertexWrapper->myStride
			, &myVertexWrapper->myByteOffset);

		D3DX11_TECHNIQUE_DESC techDesc;
		myEmitterData.myEffect->GetTechnique()->GetDesc(&techDesc);

		for (UINT i = 0; i < techDesc.Passes; ++i)
		{
			myEmitterData.myEffect->GetTechnique()->GetPassByIndex(i)->Apply(0, Engine::GetInstance()->GetContex());
			Engine::GetInstance()->GetContex()->Draw(myGraphicalParticles.Size(), 0);
		}
	}

	void EmitterInstance::Update(float aDeltaTime, const CU::Matrix44f& aWorldMatrix)
	{
		UpdateEmitter(aDeltaTime,aWorldMatrix);
	}

	bool EmitterInstance::GetIsActive()
	{
		return myIsActive;
	}

	void EmitterInstance::ToggleActive()
	{
		myIsActive = !myIsActive;
		myEmitterLife = myEmitterData.myEmitterLifeTime;
		myDeadParticleCount = 0;
	}

	void EmitterInstance::ToggleActive(bool aIsActive)
	{
		myIsActive = aIsActive;
		myEmitterLife = myEmitterData.myEmitterLifeTime;
		myDeadParticleCount = 0;
	}

	void EmitterInstance::CreateVertexBuffer()
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
		DL_ASSERT_EXP(hr == S_OK, "[EmitterInstance](CreateVertexBuffer) : Failed to create VertexBuffer");

	}

	void EmitterInstance::UpdateVertexBuffer()
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

	void EmitterInstance::UpdateEmitter(float aDeltaTime, const CU::Matrix44f& aWorldMatrix)
	{
		myEmissionTime -= aDeltaTime;
		myEmitterLife -= aDeltaTime;

		UpdateParticle(aDeltaTime);

		if (myEmissionTime <= 0.f && (myEmitterLife > 0.f || myEmitterData.myUseEmitterLifeTime == false))
		{
			EmittParticle(aWorldMatrix);
			myEmissionTime = myEmitterData.myEmissionRate;
		}
		
		if (myEmitterLife <= 0.f && myDeadParticleCount == myLogicalParticles.Size())
		{
			myIsActive = false;
		}

	}

	void EmitterInstance::UpdateParticle(float aDeltaTime)
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

			myGraphicalParticles[i].myAlpha += myEmitterData.myData.myAlphaDelta * aDeltaTime; 
			myGraphicalParticles[i].mySize += myEmitterData.myData.mySizeDelta * aDeltaTime;

			myGraphicalParticles[i].myColor += myDiffColor * aDeltaTime;

			myGraphicalParticles[i].myLifeTime -= aDeltaTime;

			
		}
	}

	void EmitterInstance::EmittParticle(const CU::Matrix44f& aWorldMatrix)
	{
		for (int i = 0; i < myEmitterData.myParticlesPerEmitt; ++i)
		{
			if (myParticleIndex == myLogicalParticles.Size() - 1)
			{
				myParticleIndex = 0;
			}
			myDeadParticleCount--;
			myLogicalParticles[myParticleIndex].myIsAlive = true;

			myLogicalParticles[myParticleIndex].myDirection = myEmitterData.myDirection;
			myGraphicalParticles[myParticleIndex].myColor = myEmitterData.myData.myStartColor;

			myGraphicalParticles[myParticleIndex].myPosition =
				CU::Math::RandomRange(aWorldMatrix.GetPos() + myEmitterData.myEmitterSize
				, aWorldMatrix.GetPos() - myEmitterData.myEmitterSize);

			myGraphicalParticles[myParticleIndex].myLifeTime = myEmitterData.myParticlesLifeTime;

			myParticleScaling = CU::Math::RandomRange(myEmitterData.myData.myMinStartSize
				, myEmitterData.myData.myMaxStartSize);

			myGraphicalParticles[myParticleIndex].mySize = 1 * myParticleScaling;

			myLogicalParticles[myParticleIndex].myVelocity.x = CU::Math::RandomRange(myEmitterData.myData.myMinSpeed.x,
				myEmitterData.myData.myMaxSpeed.x);

			myLogicalParticles[myParticleIndex].myVelocity.y = CU::Math::RandomRange(myEmitterData.myData.myMinSpeed.y,
				myEmitterData.myData.myMaxSpeed.y);

			myLogicalParticles[myParticleIndex].myVelocity.z = CU::Math::RandomRange(myEmitterData.myData.myMinSpeed.z,
				myEmitterData.myData.myMaxSpeed.z);

			myGraphicalParticles[myParticleIndex].myAlpha = myEmitterData.myData.myStartAlpha;

			myParticleIndex += 1;
		}

	}
}