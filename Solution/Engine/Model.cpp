#include "stdafx.h"

#include <d3dx11effect.h>
#include "Effect.h"
#include "EffectContainer.h"
#include "Engine.h"
#include "IndexBufferWrapper.h"
#include "Model.h"
#include "Surface.h"
#include "VertexBufferWrapper.h"
#include "VertexDataWrapper.h"
#include "VertexIndexWrapper.h"

Prism::Model::Model()
{
	myChilds.Init(2);
	myChildTransforms.Init(2);
	myVertices.Init(64);
	myVerticeIndices.Init(64);
	mySurfaces.Init(4);
	myVertexFormat.Init(2);
	myIsNULLObject = true;
	myInited = false;
	myIsLodGroup = false;
	myLodGroup = nullptr;

	myVertexBuffer = nullptr;
	myIndexBuffer = nullptr;
	myVertexBaseData = nullptr;
	myIndexBaseData = nullptr;
	myParent = nullptr;
}

Prism::Model::~Model()
{
	myChilds.DeleteAll();

	delete myVertexBuffer;
	delete myIndexBuffer;

	delete myVertexBaseData;
	delete myIndexBaseData;
	mySurfaces.DeleteAll();

	if (myVertexLayout != nullptr)
	{
		myVertexLayout->Release();
		myVertexLayout = nullptr;
	}
}

void Prism::Model::Init()
{
	DL_ASSERT_EXP(myInited == false, "Tried to Init a model twice");

	if (myIsNULLObject == false)
	{
		const int size = myVertexFormat.Size();
		D3D11_INPUT_ELEMENT_DESC* vertexDesc = new D3D11_INPUT_ELEMENT_DESC[size];
		for (int i = 0; i < myVertexFormat.Size(); ++i)
		{
			vertexDesc[i] = *myVertexFormat[i];
		}


		D3DX11_PASS_DESC passDesc;
		HRESULT hr = S_OK;
		hr = myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);

		if (hr != S_OK)
		{
			DL_MESSAGE_BOX("Model->Init(): Failed to GetDesc", "Model->Init Error", MB_ICONWARNING);
		}

		hr = Engine::GetInstance()->GetDevice()->CreateInputLayout(vertexDesc, size, 
				passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myVertexLayout);

		if (hr != S_OK)
		{
			DL_MESSAGE_BOX("Model->Init(): Failed to CreateInputLayout", "Model->Init Error", MB_ICONWARNING);
		}

		delete[] vertexDesc;

		myVertexCount = myVertexBaseData->myNumberOfVertices;
		if (InitVertexBuffer() == false)
		{
			DL_ASSERT("Model::Init() failed to InitVertexBuffer()");
		}

		if (InitIndexBuffer() == false)
		{
			DL_ASSERT("Model::Init() failed to InitIndexBuffer()");
		}
	}

	for (int i = 0; i < myChilds.Size(); ++i)
	{
		myChilds[i]->Init();
	}

	myInited = true;
}

void Prism::Model::InitPolygon()
{
	DL_ASSERT_EXP(myInited == false, "Tried to Init a model twice");

	myEffect = Engine::GetInstance()->GetEffectContainer()->GetEffect("Data/Resource/Shader/S_effect_polygon.fx");

	if (myEffect == nullptr)
	{
		DL_MESSAGE_BOX("Failed to GetEffect", "InitPolygon", MB_ICONWARNING);
	}

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3DX11_PASS_DESC passDesc;
	myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
	HRESULT hr = Engine::GetInstance()->GetDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), 
			passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myVertexLayout);
	if (FAILED(hr) != S_OK)
	{
		DL_MESSAGE_BOX("Failed to CreateInputLayout", "InitPolygon", MB_ICONWARNING);
	}

	CU::GrowingArray<VertexPosColor> vertices;
	vertices.Init(3);
	vertices.Add({ { 0.0f, 0.5f, 0.5f }, { 1.f, 0.f, 0.f, 1.f } });
	vertices.Add({ { 0.5f, -0.5f, 0.5 }, { 0.f, 1.f, 0.f, 1.f } });
	vertices.Add({ { -0.5f, -0.5f, 0.5f }, { 0.f, 0.f, 1.f, 1.f } });


	myVerticeIndices.Add(0);
	myVerticeIndices.Add(1);
	myVerticeIndices.Add(2);

	InitVertexBaseData(vertices.Size(), VertexType::POS_COLOR, sizeof(VertexPosColor), 
			reinterpret_cast<char*>(&vertices[0]));
	InitIndexBaseData(DXGI_FORMAT_R32_UINT, myVerticeIndices.Size(), reinterpret_cast<char*>(&myVerticeIndices[0]));

	InitVertexBuffer();
	InitIndexBuffer();

	Surface surf;
	surf.SetEffect(myEffect);
	surf.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	surf.SetVertexStart(0);
	surf.SetVertexCount(vertices.Size());
	surf.SetIndexStart(0);
	surf.SetIndexCount(myVerticeIndices.Size());

	mySurfaces.Add(new Surface(surf));

	myIsNULLObject = false;

	myInited = true;
}

void Prism::Model::InitCube(const float aWidth, const float aHeight, const float aDepth)
{
	DL_ASSERT_EXP(myInited == false, "Tried to Init a model twice");

	myEffect = Engine::GetInstance()->GetEffectContainer()->GetEffect("Data/Resource/Shader/S_effect_cube.fx");

	if (myEffect == nullptr)
	{
		DL_MESSAGE_BOX("Failed to GetEffect", "InitCube", MB_ICONWARNING);
		DL_ASSERT("InitCube: Failed to GetEffect, myEffect is nullptr.");
	}

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3DX11_PASS_DESC passDesc;
	myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
	HRESULT hr = Engine::GetInstance()->GetDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), 
			passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myVertexLayout);
	if (FAILED(hr) != S_OK)
	{
		DL_MESSAGE_BOX("Failed to CreateInputLayout", "InitCube", MB_ICONWARNING);
	}

	
#pragma region Vertices
	CU::GrowingArray<VertexPosNormUV> vertices;
	vertices.Init(24);

	float halfWidth = aWidth / 2.f;
	float halfHeight = aHeight / 2.f;
	float halfDepth = aDepth / 2.f;

	//0 - 3 (Top)
	vertices.Add({ { -halfWidth, halfHeight, -halfDepth }, { 0.f, 1.f, 0.f }, { 0.0f, 0.0f } });
	vertices.Add({ { halfWidth, halfHeight, -halfDepth }, { 0.f, 1.f, 0.f }, { 1.0f, 0.0f } });
	vertices.Add({ { halfWidth, halfHeight, halfDepth }, { 0.f, 1.f, 0.f }, { 1.0f, 1.0f } });
	vertices.Add({ { -halfWidth, halfHeight, halfDepth }, { 0.f, 1.f, 0.f }, { 0.0f, 1.0f } });

	//4 - 7 (Bottom)
	vertices.Add({ { -halfWidth, -halfHeight, -halfDepth }, { 0.f, -1.f, 0.f }, { 0.0f, 0.0f } });
	vertices.Add({ { halfWidth, -halfHeight, -halfDepth }, { 0.f, -1.f, 0.f }, { 1.0f, 0.0f } });
	vertices.Add({ { halfWidth, -halfHeight, halfDepth }, { 0.f, -1.f, 0.f }, { 1.0f, 1.0f } });
	vertices.Add({ { -halfWidth, -halfHeight, halfDepth }, { 0.f, -1.f, 0.f }, { 0.0f, 1.0f } });

	//8 - 11 (Left)
	vertices.Add({ { -halfWidth, -halfHeight, halfDepth }, { -1.f, 0.f, 0.f }, { 0.0f, 0.0f } });
	vertices.Add({ { -halfWidth, -halfHeight, -halfDepth }, { -1.f, 0.f, 0.f }, { 1.0f, 0.0f } });
	vertices.Add({ { -halfWidth, halfHeight, -halfDepth }, { -1.f, 0.f, 0.f }, { 1.0f, 1.0f } });
	vertices.Add({ { -halfWidth, halfHeight, halfDepth }, { -1.f, 0.f, 0.f }, { 0.0f, 1.0f } });
 
	//12 - 15 (Right)
	vertices.Add({ { halfWidth, -halfHeight, halfDepth }, { 1.f, 0.f, 0.f }, { 0.0f, 0.0f } });
	vertices.Add({ { halfWidth, -halfHeight, -halfDepth }, { 1.f, 0.f, 0.f }, { 1.0f, 0.0f } });
	vertices.Add({ { halfWidth, halfHeight, -halfDepth }, { 1.f, 0.f, 0.f }, { 1.0f, 1.0f } });
	vertices.Add({ { halfWidth, halfHeight, halfDepth }, { 1.f, 0.f, 0.f }, { 0.0f, 1.0f } });

	//16 - 19 (Front)
	vertices.Add({ { -halfWidth, -halfHeight, -halfDepth }, { 0.f, 0.f, -1.f }, { 0.0f, 0.0f } });
	vertices.Add({ { halfWidth, -halfHeight, -halfDepth }, { 0.f, 0.f, -1.f }, { 1.0f, 0.0f } });
	vertices.Add({ { halfWidth, halfHeight, -halfDepth }, { 0.f, 0.f, -1.f }, { 1.0f, 1.0f } });
	vertices.Add({ { -halfWidth, halfHeight, -halfDepth }, { 0.f, 0.f, -1.f }, { 0.0f, 1.0f } });
 
	//20 - 23 (Back)
	vertices.Add({ { -halfWidth, -halfHeight, halfDepth }, { 0.f, 0.f, 1.f }, { 0.0f, 0.0f } });
	vertices.Add({ { halfWidth, -halfHeight, halfDepth }, { 0.f, 0.f, 1.f }, { 1.0f, 0.0f } });
	vertices.Add({ { halfWidth, halfHeight, halfDepth }, { 0.f, 0.f, 1.f }, { 1.0f, 1.0f } });
	vertices.Add({ { -halfWidth, halfHeight, halfDepth }, { 0.f, 0.f, 1.f }, { 0.0f, 1.0f } });
#pragma endregion

#pragma region Indices

	//Top
	myVerticeIndices.Add(3);
	myVerticeIndices.Add(1);
	myVerticeIndices.Add(0);
				   
	myVerticeIndices.Add(2);
	myVerticeIndices.Add(1);
	myVerticeIndices.Add(3);
				   
	//Bottom
	myVerticeIndices.Add(6);
	myVerticeIndices.Add(4);
	myVerticeIndices.Add(5);
				   
	myVerticeIndices.Add(7);
	myVerticeIndices.Add(4);
	myVerticeIndices.Add(6);
				   
	//Left
	myVerticeIndices.Add(11);
	myVerticeIndices.Add(9);
	myVerticeIndices.Add(8);
				   
	myVerticeIndices.Add(10);
	myVerticeIndices.Add(9);
	myVerticeIndices.Add(11);
				   
	//Right
	myVerticeIndices.Add(14);
	myVerticeIndices.Add(12);
	myVerticeIndices.Add(13);
				   
	myVerticeIndices.Add(15);
	myVerticeIndices.Add(12);
	myVerticeIndices.Add(14);
				   
	//Front
	myVerticeIndices.Add(19);
	myVerticeIndices.Add(17);
	myVerticeIndices.Add(16);
				   
	myVerticeIndices.Add(18);
	myVerticeIndices.Add(17);
	myVerticeIndices.Add(19);
	
	//Back
	myVerticeIndices.Add(22);
	myVerticeIndices.Add(20);
	myVerticeIndices.Add(21);
				   
	myVerticeIndices.Add(23);
	myVerticeIndices.Add(20);
	myVerticeIndices.Add(22);
				   
#pragma endregion
	

	InitVertexBaseData(vertices.Size(), VertexType::POS_NORM_UV, sizeof(VertexPosNormUV), 
			reinterpret_cast<char*>(&vertices[0]));
	InitIndexBaseData(DXGI_FORMAT_R32_UINT, myVerticeIndices.Size(), reinterpret_cast<char*>(&myVerticeIndices[0]));

	InitVertexBuffer();
	InitIndexBuffer();

	Surface surf;
	surf.SetEffect(myEffect);
	surf.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	surf.SetVertexStart(0);
	surf.SetVertexCount(myVertices.Size());
	surf.SetIndexStart(0);
	surf.SetIndexCount(myVerticeIndices.Size());
	surf.SetTexture("DiffuseTexture", "Data/Resource/Texture/Debug/T_debug_seafloor.dds", true);

	mySurfaces.Add(new Surface(surf));

	myIsNULLObject = false;
	myInited = true;
}

void Prism::Model::InitLightCube(const float aWidth, const float aHeight, const float aDepth, CU::Vector4f aColour)
{
	DL_ASSERT_EXP(myInited == false, "Tried to Init a model twice");

	myEffect = Engine::GetInstance()->GetEffectContainer()->GetEffect("Data/Resource/Shader/S_effect_cube_colored.fx");

	if (myEffect == nullptr)
	{
		DL_MESSAGE_BOX("Failed to GetEffect", "InitLightCube", MB_ICONWARNING);
	}

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3DX11_PASS_DESC passDesc;
	myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
	HRESULT hr = Engine::GetInstance()->GetDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc),
		passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myVertexLayout);
	if (FAILED(hr) != S_OK)
	{
		DL_MESSAGE_BOX("Failed to CreateInputLayout", "InitLightCube", MB_ICONWARNING);
	}


#pragma region Vertices
	CU::GrowingArray<VertexPosColor> vertices;
	vertices.Init(24);

	float halfWidth = aWidth / 2.f;
	float halfHeight = aHeight / 2.f;
	float halfDepth = aDepth / 2.f;

	//0 - 3 (Top)
	vertices.Add({ { -halfWidth, halfHeight, -halfDepth },aColour });
	vertices.Add({ { halfWidth, halfHeight, -halfDepth }, aColour });
	vertices.Add({ { halfWidth, halfHeight, halfDepth }, aColour });
	vertices.Add({ { -halfWidth, halfHeight, halfDepth }, aColour });

	//4 - 7 (Bottom)
	vertices.Add({ { -halfWidth, -halfHeight, -halfDepth }, aColour });
	vertices.Add({ { halfWidth, -halfHeight, -halfDepth }, aColour });
	vertices.Add({ { halfWidth, -halfHeight, halfDepth }, aColour });
	vertices.Add({ { -halfWidth, -halfHeight, halfDepth }, aColour });

	//8 - 11 (Left)
	vertices.Add({ { -halfWidth, -halfHeight, halfDepth }, aColour });
	vertices.Add({ { -halfWidth, -halfHeight, -halfDepth }, aColour });
	vertices.Add({ { -halfWidth, halfHeight, -halfDepth }, aColour });
	vertices.Add({ { -halfWidth, halfHeight, halfDepth }, aColour });

	//12 - 15 (Right)
	vertices.Add({ { halfWidth, -halfHeight, halfDepth }, aColour });
	vertices.Add({ { halfWidth, -halfHeight, -halfDepth }, aColour });
	vertices.Add({ { halfWidth, halfHeight, -halfDepth }, aColour });
	vertices.Add({ { halfWidth, halfHeight, halfDepth }, aColour });

	//16 - 19 (Front)
	vertices.Add({ { -halfWidth, -halfHeight, -halfDepth }, aColour });
	vertices.Add({ { halfWidth, -halfHeight, -halfDepth }, aColour });
	vertices.Add({ { halfWidth, halfHeight, -halfDepth }, aColour });
	vertices.Add({ { -halfWidth, halfHeight, -halfDepth }, aColour });

	//20 - 23 (Back)
	vertices.Add({ { -halfWidth, -halfHeight, halfDepth }, aColour });
	vertices.Add({ { halfWidth, -halfHeight, halfDepth }, aColour });
	vertices.Add({ { halfWidth, halfHeight, halfDepth }, aColour });
	vertices.Add({ { -halfWidth, halfHeight, halfDepth }, aColour });
#pragma endregion

#pragma region Indices

	//Top
	myVerticeIndices.Add(3);
	myVerticeIndices.Add(1);
	myVerticeIndices.Add(0);

	myVerticeIndices.Add(2);
	myVerticeIndices.Add(1);
	myVerticeIndices.Add(3);

	//Bottom
	myVerticeIndices.Add(6);
	myVerticeIndices.Add(4);
	myVerticeIndices.Add(5);

	myVerticeIndices.Add(7);
	myVerticeIndices.Add(4);
	myVerticeIndices.Add(6);

	//Left
	myVerticeIndices.Add(11);
	myVerticeIndices.Add(9);
	myVerticeIndices.Add(8);

	myVerticeIndices.Add(10);
	myVerticeIndices.Add(9);
	myVerticeIndices.Add(11);

	//Right
	myVerticeIndices.Add(14);
	myVerticeIndices.Add(12);
	myVerticeIndices.Add(13);

	myVerticeIndices.Add(15);
	myVerticeIndices.Add(12);
	myVerticeIndices.Add(14);

	//Front
	myVerticeIndices.Add(19);
	myVerticeIndices.Add(17);
	myVerticeIndices.Add(16);

	myVerticeIndices.Add(18);
	myVerticeIndices.Add(17);
	myVerticeIndices.Add(19);

	//Back
	myVerticeIndices.Add(22);
	myVerticeIndices.Add(20);
	myVerticeIndices.Add(21);

	myVerticeIndices.Add(23);
	myVerticeIndices.Add(20);
	myVerticeIndices.Add(22);

#pragma endregion


	InitVertexBaseData(vertices.Size(), VertexType::POS_NORM_UV, sizeof(VertexPosColor),
		reinterpret_cast<char*>(&vertices[0]));
	InitIndexBaseData(DXGI_FORMAT_R32_UINT, myVerticeIndices.Size(), reinterpret_cast<char*>(&myVerticeIndices[0]));

	InitVertexBuffer();
	InitIndexBuffer();

	Surface surf;
	surf.SetEffect(myEffect);
	surf.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	surf.SetVertexStart(0);
	surf.SetVertexCount(myVertices.Size());
	surf.SetIndexStart(0);
	surf.SetIndexCount(myVerticeIndices.Size());
	surf.SetTexture("DiffuseTexture", "Data/Resource/Texture/Debug/T_debug_seafloor.dds", true);

	mySurfaces.Add(new Surface(surf));

	myIsNULLObject = false;

	myInited = true;
}

void Prism::Model::AddChild(Model* aChild)
{
	aChild->myParent = this;
	myChilds.Add(aChild);
	myChildTransforms.Add(aChild->myOrientation);
}

void Prism::Model::SetLodGroup(LodGroup* aLodGroup)
{
	myLodGroup = aLodGroup;
	if (myLodGroup != nullptr)
	{
		myIsLodGroup = true;
	}
}

Prism::Effect* Prism::Model::GetEffect()
{
	return myEffect;
}

void Prism::Model::SetEffect(Effect* aEffect)
{
	myEffect = aEffect;
	myEffect->AddListener(this);

	for (int i = 0; i < mySurfaces.Size(); ++i)
	{
		mySurfaces[i]->SetEffect(myEffect);
		mySurfaces[i]->ReloadSurface();
	}

	for (int i = 0; i < myChilds.Size(); ++i) 
	{
		myChilds[i]->SetEffect(aEffect);
	}
}

void Prism::Model::Render(const CU::Matrix44<float>& aOrientation, const CU::Vector3<float>& aCameraPosition)
{
	if (myIsLodGroup == true)
	{
		float lengthBetweenCameraAndModel = CU::Length(aCameraPosition - aOrientation.GetPos());
		int level = 0;

		Model* toRender = nullptr;
		for (int i = myChilds.Size() - 1; i >= 0; i--)
		{
			LodGroup* group = myLodGroup;
			double threashold = group->myThreshHolds[i];
			if (threashold <= lengthBetweenCameraAndModel)
			{
				toRender = myChilds[i];
				level = i;
				break;
			}
		}

		if (toRender)
		{
			toRender->Render(aOrientation, aCameraPosition);
		}
	}
	else
	{
		if (myIsNULLObject == false)
		{
			TIME_FUNCTION;

			float blendFactor[4];
			blendFactor[0] = 0.f;
			blendFactor[1] = 0.f;
			blendFactor[2] = 0.f;
			blendFactor[3] = 0.f;

			myEffect->SetBlendState(NULL, blendFactor);
			myEffect->SetWorldMatrix(aOrientation);


			Engine::GetInstance()->GetContex()->IASetInputLayout(myVertexLayout);
			Engine::GetInstance()->GetContex()->IASetVertexBuffers(myVertexBuffer->myStartSlot,
				myVertexBuffer->myNumberOfBuffers, &myVertexBuffer->myVertexBuffer,
				&myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);
			Engine::GetInstance()->GetContex()->IASetIndexBuffer(myIndexBuffer->myIndexBuffer,
				myIndexBuffer->myIndexBufferFormat, myIndexBuffer->myByteOffset);




			for (int s = 0; s < mySurfaces.Size(); ++s)
			{
				mySurfaces[s]->Activate();

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
				DL_ASSERT_EXP(tech->IsValid(), "Technique is not valid");
				tech->GetDesc(&techDesc);
				for (UINT i = 0; i < techDesc.Passes; ++i)
				{
					tech->GetPassByIndex(i)->Apply(0,
						Engine::GetInstance()->GetContex());
					Engine::GetInstance()->GetContex()->DrawIndexed(mySurfaces[s]->GetIndexCount(),
						mySurfaces[s]->GetVertexStart(), 0);
				}
			}
		}

		for (int i = 0; i < myChilds.Size(); ++i)
		{
			myChilds[i]->Render(myChildTransforms[i] * aOrientation, aCameraPosition);
		}
	}
}

void Prism::Model::OnEffectLoad()
{
	for (int i = 0; i < mySurfaces.Size(); ++i)
	{
		mySurfaces[i]->ReloadSurface();
	}
}

void Prism::Model::InitVertexBaseData(int aNumberOfVertices, VertexType aVertexType, int aVertexSize, char* aVertexData)
{
	myVertexBaseData = new VertexDataWrapper();
	myVertexBaseData->myNumberOfVertices = aNumberOfVertices;
	myVertexBaseData->myStride = aVertexSize;
	myVertexBaseData->myType = aVertexType;
	myVertexBaseData->mySize = aVertexSize * myVertexBaseData->myNumberOfVertices;
	myVertexBaseData->myVertexData = aVertexData;
}

void Prism::Model::InitIndexBaseData(DXGI_FORMAT aFormat, int aNumberOfIndices, char* aIndexData)
{
	myIndexBaseData = new VertexIndexWrapper();
	myIndexBaseData->myFormat = aFormat;
	myIndexBaseData->myNumberOfIndices = aNumberOfIndices;
	myIndexBaseData->myIndexData = aIndexData;
	myIndexBaseData->mySize = sizeof(UINT) * myIndexBaseData->myNumberOfIndices;
}

bool Prism::Model::InitVertexBuffer()
{
	myVertexBuffer = new VertexBufferWrapper();

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = myVertexBaseData->mySize;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = myVertexBaseData->myStride;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));

	InitData.pSysMem = myVertexBaseData->myVertexData;
	
	HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBuffer(&bd, &InitData, 
			&myVertexBuffer->myVertexBuffer);
	if (FAILED(hr) != S_OK)
		return false;

	myVertexBuffer->myStride = myVertexBaseData->myStride;
	myVertexBuffer->myByteOffset = 0;
	myVertexBuffer->myStartSlot = 0;
	myVertexBuffer->myNumberOfBuffers = 1;

	return true;
}

bool Prism::Model::InitIndexBuffer()
{
	myIndexBuffer = new IndexBufferWrapper();

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = myIndexBaseData->mySize;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));

	InitData.pSysMem = myIndexBaseData->myIndexData;

	HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBuffer(&bd, &InitData, &myIndexBuffer->myIndexBuffer);
	if (FAILED(hr) != S_OK)
		return false;

	myIndexBuffer->myIndexBufferFormat = myIndexBaseData->myFormat;
	myIndexBuffer->myByteOffset = 0;

	return true;
}
