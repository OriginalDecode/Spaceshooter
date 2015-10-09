#include "stdafx.h"
#include "EmitterInstance.h"
#include "VertexBufferWrapper.h"
namespace Prism
{
	void EmitterInstance::Initiate(EmitterData& someData)
	{

	}

	void EmitterInstance::Render(Camera* aCamera)
	{

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