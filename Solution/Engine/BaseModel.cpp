#include "stdafx.h"

#include "BaseModel.h"
#include <D3D11.h>
#include <d3dx11effect.h>
#include "Effect.h"
#include "IndexBufferWrapper.h"
#include "Surface.h"


namespace Prism
{

	BaseModel::BaseModel()
	{
		myVertexBufferDesc = new D3D11_BUFFER_DESC();
		myInstanceBufferDesc = new D3D11_BUFFER_DESC();
		myIndexBufferDesc = new D3D11_BUFFER_DESC();
		myInitData = new D3D11_SUBRESOURCE_DATA();

		myEffect = nullptr;

		mySurfaces.Init(1);

		myVertexBuffer = nullptr;
		myIndexBuffer = nullptr;

	}

	BaseModel::~BaseModel()
	{
		if (myVertexBuffer != nullptr && myVertexBuffer->myVertexBuffer != nullptr)
		{
			myVertexBuffer->myVertexBuffer->Release();
			delete myVertexBuffer;
		}

		if (myIndexBuffer != nullptr && myIndexBuffer->myIndexBuffer != nullptr)
		{
			myIndexBuffer->myIndexBuffer->Release();
			delete myIndexBuffer;
		}

		if (myVertexLayout != nullptr)
		{
			myVertexLayout->Release();
			myVertexLayout = nullptr;
		}

		delete myVertexBufferDesc;
		delete myInstanceBufferDesc;
		delete myIndexBufferDesc;
		delete myInitData;
		mySurfaces.DeleteAll();
	}

	void BaseModel::BeginRender(const CU::GrowingArray<CU::Matrix44<float>>& someOrientations)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		Engine::GetInstance()->GetContex()->Map(myInstanceBuffer->myVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (mappedResource.pData != nullptr)
		{
			CU::Matrix44f* data = (CU::Matrix44f*)mappedResource.pData;
			for (int i = 0; i < someOrientations.Size(); ++i)
			{
				data[i] = someOrientations[i];
			}
		}

		Engine::GetInstance()->GetContex()->Unmap(myInstanceBuffer->myVertexBuffer, 0);
		UINT strides[2] = { myVertexBuffer->myStride, sizeof(CU::Matrix44f) };
		UINT offsets[2] = { 0, 0 };

		myVertexBuffers[0] = myVertexBuffer->myVertexBuffer;
		myVertexBuffers[1] = myInstanceBuffer->myVertexBuffer;

		Engine::GetInstance()->GetContex()->IASetVertexBuffers(0, 2, myVertexBuffers, strides, offsets);

		Engine::GetInstance()->GetContex()->IASetInputLayout(myVertexLayout);
		//Engine::GetInstance()->GetContex()->IASetVertexBuffers(myVertexBuffer->myStartSlot
		//	, myVertexBuffer->myNumberOfBuffers, &myVertexBuffer->myVertexBuffer
		//	, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);
		Engine::GetInstance()->GetContex()->IASetIndexBuffer(myIndexBuffer->myIndexBuffer
			, myIndexBuffer->myIndexBufferFormat, myIndexBuffer->myByteOffset);
		for (int s = 0; s < mySurfaces.Size(); ++s)
		{
			mySurfaces[s]->Activate();
		}
	}

	void BaseModel::BeginRenderNormal()
	{

		D3D11_MAPPED_SUBRESOURCE mappedResource;

		Engine::GetInstance()->GetContex()->Map(myInstanceBuffer->myVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (mappedResource.pData != nullptr)
		{
			CU::Matrix44f* data = (CU::Matrix44f*)mappedResource.pData;
			for (int i = 0; i < 0; ++i)
			{
				//data[i] = someOrientations[i];
			}
		}

		Engine::GetInstance()->GetContex()->Unmap(myInstanceBuffer->myVertexBuffer, 0);
		UINT strides[2] = { myVertexBuffer->myStride, sizeof(CU::Matrix44f) };
		UINT offsets[2] = { 0, 0 };

		myVertexBuffers[0] = myVertexBuffer->myVertexBuffer;
		myVertexBuffers[1] = myInstanceBuffer->myVertexBuffer;

		Engine::GetInstance()->GetContex()->IASetVertexBuffers(0, 2, myVertexBuffers, strides, offsets);

		//Engine::GetInstance()->GetContex()->IASetVertexBuffers(0, 1, myVertexBuffers, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);

		Engine::GetInstance()->GetContex()->IASetInputLayout(myVertexLayout);
		Engine::GetInstance()->GetContex()->IASetVertexBuffers(myVertexBuffer->myStartSlot
			, 1, &myVertexBuffer->myVertexBuffer
			, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);
		Engine::GetInstance()->GetContex()->IASetIndexBuffer(myIndexBuffer->myIndexBuffer
			, myIndexBuffer->myIndexBufferFormat, myIndexBuffer->myByteOffset);
		for (int s = 0; s < mySurfaces.Size(); ++s)
		{
			mySurfaces[s]->Activate();
		}
	}

	void BaseModel::Render()
	{
		for (int s = 0; s < mySurfaces.Size(); ++s)
		{
			ID3DX11EffectTechnique* tech = nullptr;
			D3DX11_TECHNIQUE_DESC techDesc;

			if (mySurfaces[s]->GetEmissive() == true)
			{
				tech = myEffect->GetEffect()->GetTechniqueByName("Render_Emissive");
			}
			else
			{
				tech = myEffect->GetTechnique();
			}

			DL_ASSERT_EXP(tech != nullptr, "Technique is nullptr");
			DL_ASSERT_EXP(tech->IsValid() != false, "Technique is not valid");

			tech->GetDesc(&techDesc);
			for (UINT i = 0; i < techDesc.Passes; ++i)
			{
				tech->GetPassByIndex(i)->Apply(0, Engine::GetInstance()->GetContex());
				Engine::GetInstance()->GetContex()->DrawIndexed(mySurfaces[s]->GetIndexCount()
					, mySurfaces[s]->GetVertexStart(), 0);
			}
		}
	}

	void BaseModel::InitInputLayout(D3D11_INPUT_ELEMENT_DESC* aVertexDescArray, int aArraySize)
	{
		D3DX11_PASS_DESC passDesc;
		myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
		HRESULT hr = Engine::GetInstance()->GetDevice()->CreateInputLayout(aVertexDescArray
			, aArraySize, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myVertexLayout);
		if (FAILED(hr) != S_OK)
		{
			DL_MESSAGE_BOX("Failed to CreateInputLayout", "BaseModel::Init", MB_ICONWARNING);
		}
	}

	void BaseModel::InitVertexBuffer(int aVertexSize, int aBufferUsage, int aCPUUsage)
	{
		myVertexBuffer = new VertexBufferWrapper();
		myVertexBuffer->myStride = aVertexSize;
		myVertexBuffer->myByteOffset = 0;
		myVertexBuffer->myStartSlot = 0;
		myVertexBuffer->myNumberOfBuffers = 2;


		ZeroMemory(myVertexBufferDesc, sizeof(myVertexBufferDesc));
		myVertexBufferDesc->Usage = static_cast<D3D11_USAGE>(aBufferUsage);
		myVertexBufferDesc->BindFlags = D3D11_BIND_VERTEX_BUFFER;
		myVertexBufferDesc->CPUAccessFlags = aCPUUsage;
		myVertexBufferDesc->MiscFlags = 0;
		myVertexBufferDesc->StructureByteStride = 0;
	}

	void BaseModel::InitInstanceBuffer(int aVertexSize, int aBufferUsage, int aCPUUsage)
	{
		myInstanceBuffer = new VertexBufferWrapper();
		myInstanceBuffer->myStride = aVertexSize; // or 1024x too small
		myInstanceBuffer->myByteOffset = 0;
		myInstanceBuffer->myStartSlot = 0;
		myInstanceBuffer->myNumberOfBuffers = 2;


		ZeroMemory(myInstanceBufferDesc, sizeof(myInstanceBufferDesc));
		myInstanceBufferDesc->Usage = static_cast<D3D11_USAGE>(aBufferUsage);
		myInstanceBufferDesc->BindFlags = D3D11_BIND_VERTEX_BUFFER;
		myInstanceBufferDesc->CPUAccessFlags = aCPUUsage;
		myInstanceBufferDesc->MiscFlags = 0;
		myInstanceBufferDesc->StructureByteStride = 0;
	}

	void BaseModel::InitIndexBuffer()
	{
		myIndexBuffer = new IndexBufferWrapper();
		myIndexBuffer->myIndexBufferFormat = DXGI_FORMAT_R32_UINT;
		myIndexBuffer->myByteOffset = 0;


		ZeroMemory(myIndexBufferDesc, sizeof(myIndexBufferDesc));
		myIndexBufferDesc->Usage = D3D11_USAGE_IMMUTABLE;
		myIndexBufferDesc->BindFlags = D3D11_BIND_INDEX_BUFFER;
		myIndexBufferDesc->CPUAccessFlags = 0;
		myIndexBufferDesc->MiscFlags = 0;
		myIndexBufferDesc->StructureByteStride = 0;
	}

	void BaseModel::InitSurface(const std::string& aResourceName, const std::string& aFileName)
	{
		Surface* surface = new Surface();

		surface->SetEffect(myEffect);
		surface->SetIndexCount(0);
		surface->SetIndexStart(0);
		surface->SetVertexCount(0);
		surface->SetVertexStart(0);
		surface->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		surface->SetTexture(aResourceName, aFileName, true);
		mySurfaces.Add(surface);
	}

	void BaseModel::InitBlendState()
	{
		D3D11_BLEND_DESC blendDesc;
		blendDesc.AlphaToCoverageEnable = true;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBlendState(&blendDesc, &myBlendState);
		if (FAILED(hr) != S_OK)
		{
			DL_ASSERT("BaseModel::InitBlendState: Failed to CreateBlendState");
		}
	}

	void BaseModel::SetupVertexBuffer(int aVertexCount, int aVertexSize, char* aVertexData)
	{
		if (myVertexBuffer->myVertexBuffer != nullptr)
			myVertexBuffer->myVertexBuffer->Release();

		myVertexBufferDesc->ByteWidth = aVertexSize * aVertexCount;
		myInitData->pSysMem = aVertexData;


		HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBuffer(myVertexBufferDesc, myInitData
			, &myVertexBuffer->myVertexBuffer);
		if (FAILED(hr) != S_OK)
		{
			DL_ASSERT("BaseModel::SetupVertexBuffer: Failed to SetupVertexBuffer");
		}
	}

	void BaseModel::SetupInstanceBuffer(int aVertexCount, int aVertexSize)
	{
		if (myInstanceBuffer->myVertexBuffer != nullptr)
			myInstanceBuffer->myVertexBuffer->Release();

		myInstanceBufferDesc->ByteWidth = aVertexSize * aVertexCount;


		HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBuffer(myInstanceBufferDesc, nullptr
			, &myInstanceBuffer->myVertexBuffer);
		if (FAILED(hr) != S_OK)
		{
			DL_ASSERT("BaseModel::SetupVertexBuffer: Failed to SetupVertexBuffer");
		}
	}

	void BaseModel::SetupIndexBuffer(int aIndexCount, char* aIndexData)
	{
		if (myIndexBuffer->myIndexBuffer != nullptr)
			myIndexBuffer->myIndexBuffer->Release();

		myIndexBufferDesc->ByteWidth = sizeof(UINT) * aIndexCount;
		myInitData->pSysMem = aIndexData;


		HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBuffer(myIndexBufferDesc, myInitData,
			&myIndexBuffer->myIndexBuffer);
		if (FAILED(hr) != S_OK)
		{
			DL_ASSERT("BaseModel::SetupIndexBuffer: Failed to SetupIndexBuffer");
		}
	}

	void BaseModel::OnEffectLoad()
	{
		for (int i = 0; i < mySurfaces.Size(); ++i)
		{
			mySurfaces[i]->ReloadSurface();
		}
	}
}