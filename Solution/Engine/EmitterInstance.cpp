#include "stdafx.h"
#include "Camera.h"
#include <d3dx11effect.h>
#include "EmitterInstance.h"
#include <MathHelper.h>
#include "VertexBufferWrapper.h"
namespace Prism
{
	EmitterInstance::~EmitterInstance()
	{
		delete myVertexWrapper;
		myVertexWrapper = nullptr;
	}

	void EmitterInstance::Initiate(EmitterData someData)
	{
		myIsActive = false;

		myEmitterData = someData;
		myParticleIndex = 0;
		myEmissionRate = 1 / myEmitterData.myEmissionRate;

		int particleCount = static_cast<int>(myEmitterData.myParticlesLifeTime / myEmissionRate) + 1;

		myGraphicalParticles.Init(particleCount);
		myLogicalParticles.Init(particleCount);

		myEmissionTime = 1;

		myDiffColor = (myEmitterData.myData.myEndColor - myEmitterData.myData.myStartColor) / myEmitterData.myParticlesLifeTime;

		for (int i = 0; i < particleCount; ++i)
		{
			GraphicalParticle tempGraphics;
			myGraphicalParticles.Add(tempGraphics);
			LogicalParticle tempLogic;
			myLogicalParticles.Add(tempLogic);
		}

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
		//myOrientation *= aWorldMatrix;
		UpdateEmitter(aDeltaTime,aWorldMatrix);
	}

	bool EmitterInstance::GetIsActive()
	{
		return myIsActive;
	}

	void EmitterInstance::ToggleActive()
	{
		myIsActive = !myIsActive;
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

		UpdateParticle(aDeltaTime);

		if (myEmissionTime <= 0.f)
		{
			EmittParticle(aWorldMatrix);
		}

		myEmissionTime = myEmissionRate;
		
	}

	void EmitterInstance::UpdateParticle(float aDeltaTime)
	{
		for (int i = 0; i < myLogicalParticles.Size(); ++i)
		{
			if (myGraphicalParticles[i].myAlpha <= 0.0f)
			{
				continue;
			}
			
			myGraphicalParticles[i].myPosition = myGraphicalParticles[i].myPosition -
				myLogicalParticles[i].myVelocity * aDeltaTime;

			float alphaDiff = 0 - myGraphicalParticles[i].myAlpha;
			myGraphicalParticles[i].myAlpha += (alphaDiff * aDeltaTime) / myGraphicalParticles[i].myLifeTime;

			float sizeDiff = 0 - myGraphicalParticles[i].mySize;
			myGraphicalParticles[i].mySize += (sizeDiff * aDeltaTime) / myGraphicalParticles[i].myLifeTime;

			myGraphicalParticles[i].myColor += myDiffColor * aDeltaTime;

			myGraphicalParticles[i].myLifeTime -= aDeltaTime;

		}
	}

	void EmitterInstance::EmittParticle( const CU::Matrix44f& aWorldMatrix)
	{
		for (int i = 0; i < myEmitterData.myParticlesPerEmitt; ++i)
		{
			if (myParticleIndex == myLogicalParticles.Size() - 1)
			{
				myParticleIndex = 0;
			}

			myGraphicalParticles[myParticleIndex].myColor = myEmitterData.myData.myStartColor;

			myGraphicalParticles[myParticleIndex].myPosition =
				CU::Math::RandomRange(aWorldMatrix.GetPos() + myEmitterData.myEmitterSize
				, aWorldMatrix.GetPos() - myEmitterData.myEmitterSize);

			myGraphicalParticles[myParticleIndex].myLifeTime = myEmitterData.myParticlesLifeTime;

			myParticleScaling = CU::Math::RandomRange(myEmitterData.myData.myMinStartSize
				, myEmitterData.myData.myMaxStartSize);

			myGraphicalParticles[myParticleIndex].mySize = 1 * myParticleScaling;

			myLogicalParticles[myParticleIndex].myVelocity.x = CU::Math::RandomRange(myEmitterData.myData.myMinVelocity.x,
				myEmitterData.myData.myMaxVelocity.x);

			myLogicalParticles[myParticleIndex].myVelocity.y = CU::Math::RandomRange(myEmitterData.myData.myMinVelocity.y,
				myEmitterData.myData.myMaxVelocity.y);

			myLogicalParticles[myParticleIndex].myVelocity.z = CU::Math::RandomRange(myEmitterData.myData.myMinVelocity.z,
				myEmitterData.myData.myMaxVelocity.z);

			myGraphicalParticles[myParticleIndex].myAlpha = 1;

			myParticleIndex += 1;
		}

	}
}