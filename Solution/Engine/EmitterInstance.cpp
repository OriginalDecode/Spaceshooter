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
		myEmitterData = someData;
		myLiveParticles = 0;


		int particleCount = static_cast<int>(myEmitterData.myEmissionLifeTime / myEmitterData.myEmissionRate) + 1;

		myParticles.Init(particleCount);
		myEmissionTime = myEmitterData.myEmissionRate;

		for (int i = 0; i < myParticles.GetCapacity(); ++i)
		{
			ParticleInstance tempParticle;
			myParticles.Add(tempParticle);
		}

		myOrientation.SetPos(CU::Vector3f(0.0f, 0.0f, 5.0f));

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
			Engine::GetInstance()->GetContex()->Draw(myParticles.Size(), 0);
		}
	}

	void EmitterInstance::Update(float aDeltaTime)
	{
		UpdateEmitter(aDeltaTime);
	}

	void EmitterInstance::CreateVertexBuffer()
	{

		myVertexWrapper = new VertexBufferWrapper();
		myVertexWrapper->myStride = sizeof(ParticleInstance);
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

		vertexBufferDesc.ByteWidth = sizeof(ParticleInstance) * myEmitterData.myMaxParticleAmount;

		D3D11_SUBRESOURCE_DATA vertexData;
		ZeroMemory(&vertexData, sizeof(vertexData));

		vertexData.pSysMem = reinterpret_cast<char*>(&myParticles[0]);

		hr = Engine::GetInstance()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData
			, &myVertexWrapper->myVertexBuffer);
		DL_ASSERT_EXP(hr == S_OK, "[EmitterInstance](CreateVertexBuffer) : Failed to create VertexBuffer");

	}

	void EmitterInstance::UpdateVertexBuffer()
	{
		if (myParticles.Size() > 0)
		{

			D3D11_MAPPED_SUBRESOURCE mappedResource;

			Engine::GetInstance()->GetContex()->Map(myVertexWrapper->myVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

			if (mappedResource.pData != nullptr)
			{
				ParticleInstance *data = reinterpret_cast<ParticleInstance*>(mappedResource.pData);
				memcpy(data, &myParticles[0], sizeof(ParticleInstance)* myParticles.GetCapacity());
			}

			Engine::GetInstance()->GetContex()->Unmap(myVertexWrapper->myVertexBuffer, 0);

		}
	}

	void EmitterInstance::UpdateEmitter(float aDeltaTime)
	{
		myEmissionTime -= aDeltaTime;

		UpdateParticle(aDeltaTime);

		if (myEmissionTime <= 0)
		{
			EmittParticle();
			myEmissionTime = myEmitterData.myEmissionRate;
		}


	}

	void EmitterInstance::UpdateParticle(float aDeltaTime)
	{
		if (myLiveParticles == myParticles.Size() - 1)
		{
			myLiveParticles = 0;
		}

		for (int i = 0; i < myParticles.Size(); ++i)
		{
			myParticles[i].myAlpha += aDeltaTime * myEmitterData.myData.myAlphaDelta;
			myParticles[i].mySize += aDeltaTime * myEmitterData.myData.mySizeDelta;

			myParticles[i].myLifeTime -= aDeltaTime;
			
			myParticles[i].myPosition = myParticles[i].myPosition - myParticles[i].myVelocity * aDeltaTime;

			if (myParticles[i].myLifeTime <= 0.0f)
			{
				myParticles[i].myIsAlive = false;
				continue;
			}

			if (myParticles[i].myAlpha <= 0.0f)
			{
				myParticles[i].myIsAlive = false;
				continue;
			}
		}
	}

	void EmitterInstance::EmittParticle()
	{
		

			myParticles[myLiveParticles].myIsAlive = true;
			myParticles[myLiveParticles].myPosition = myOrientation.GetPos();
			myParticles[myLiveParticles].myLifeTime = myEmitterData.myEmissionLifeTime;

			myParticles[myLiveParticles].mySize = CU::Math::RandomRange(myEmitterData.myData.myMinStartSize
				, myEmitterData.myData.myMaxStartSize);

		//	myParticles[myLiveParticles].mySize = 100;


			myParticles[myLiveParticles].myVelocity.x = CU::Math::RandomRange(myEmitterData.myData.myMinVelocity.x,
				myEmitterData.myData.myMaxVelocity.x);

			myParticles[myLiveParticles].myVelocity.y = CU::Math::RandomRange(myEmitterData.myData.myMinVelocity.y,
				myEmitterData.myData.myMaxVelocity.y);

			myParticles[myLiveParticles].myVelocity.z = CU::Math::RandomRange(myEmitterData.myData.myMinVelocity.z,
				myEmitterData.myData.myMaxVelocity.z);

			myParticles[myLiveParticles].myAlpha = 1;
			myLiveParticles += 1;

		
	}
}