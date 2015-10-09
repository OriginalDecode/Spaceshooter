#include "stdafx.h"
#include "Camera.h"
#include <d3dx11effect.h>
#include "EmitterInstance.h"
#include "VertexBufferWrapper.h"
namespace Prism
{
	EmitterInstance::~EmitterInstance()
	{
		delete myVertexWrapper;
		myVertexWrapper = nullptr;
	}

	void EmitterInstance::Initiate(EmitterData& someData)
	{
		myData = someData;
		myParticles.Init(myData.myMaxSize);
		EmittParticle();

		myEmissionTime = myData.myEmissionRate;
		CreateVertexBuffer();
	}

	void EmitterInstance::Render(Camera* aCamera)
	{
		UpdateVertexBuffer();

		myData.myEffect->SetWorldMatrix(myOrientation);
		myData.myEffect->SetViewMatrix(aCamera->GetOrientation());
		myData.myEffect->SetProjectionMatrix(aCamera->GetProjection());

		Engine::GetInstance()->GetContex()->IASetInputLayout(myData.myInputLayout);
		Engine::GetInstance()->GetContex()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		Engine::GetInstance()->GetContex()->IASetVertexBuffers(
			  myVertexWrapper->myStartSlot
			, myVertexWrapper->myNumberOfBuffers
			, &myVertexWrapper->myVertexBuffer
			, &myVertexWrapper->myStride
			, &myVertexWrapper->myByteOffset);

		D3DX11_TECHNIQUE_DESC techDesc;
		myData.myEffect->GetTechnique()->GetDesc(&techDesc);

		for (UINT i = 0; i < techDesc.Passes; ++i)
		{
			myData.myEffect->GetTechnique()->GetPassByIndex(i)->Apply(0, Engine::GetInstance()->GetContex());
			Engine::GetInstance()->GetContex()->Draw(myParticles.Size(), 0);
		}
	}

	void EmitterInstance::Update()
	{

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

		
		D3D11_SUBRESOURCE_DATA vertexData;
		ZeroMemory(&vertexData, sizeof(vertexData));
		
		vertexData.pSysMem = reinterpret_cast<char*>(&myParticles[0]);

		hr = Engine::GetInstance()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &myVertexWrapper->myVertexBuffer);
		DL_ASSERT_EXP(FAILED(hr), "[EmitterInstance](CreateVertexBuffer) : Failed to create VertexBuffer");

	}

	void EmitterInstance::UpdateVertexBuffer()
	{
		if (myParticles.Size() > 0)
		{

			D3D11_MAPPED_SUBRESOURCE mappedResource;

			Engine::GetInstance()->GetContex()->Map(myVertexWrapper->myVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

			if (mappedResource.pData != nullptr)
			{
				ParticleData *data = reinterpret_cast<ParticleData*>(mappedResource.pData);
				memcpy(data, &myParticles[0], sizeof(ParticleData)* myParticles.GetCapacity());
			}

			Engine::GetInstance()->GetContex()->Unmap(myVertexWrapper->myVertexBuffer, 0);

		}
	}
}