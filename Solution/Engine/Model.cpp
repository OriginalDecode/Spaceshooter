#include "stdafx.h"

#include <CommonHelper.h>
#include <d3dx11effect.h>
#include "Effect.h"
#include "EffectContainer.h"
#include "Engine.h"
#include "IndexBufferWrapper.h"
#include "Model.h"
#include "RenderProcessTarget.h"
#include "Surface.h"
#include "VertexBufferWrapper.h"
#include "VertexDataWrapper.h"
#include "VertexIndexWrapper.h"

Prism::Model::Model()
	: BaseModel()
	, myChildren(2)
	, myChildTransforms(2)
	, myVertexFormat(4)
	, myIsNULLObject(true)
	, myVertexBaseData(nullptr)
	, myIndexBaseData(nullptr)
	, myIsLodGroup(false)
	, myLodGroup(nullptr)
	, myInited(false)
	, myParent(nullptr)
	, myVertexCount(0)
{
}

Prism::Model::~Model()
{
	myChildren.DeleteAll();
	delete myVertexBaseData;
	delete myIndexBaseData;
}

void Prism::Model::Init()
{
	DL_ASSERT_EXP(myInited == false, "Tried to Init a model twice");

	if (myIsNULLObject == false)
	{
		const int size = myVertexFormat.Size() + 4;
		D3D11_INPUT_ELEMENT_DESC* vertexDesc = new D3D11_INPUT_ELEMENT_DESC[size];
		for (int i = 0; i < myVertexFormat.Size(); ++i)
		{
			vertexDesc[i] = *myVertexFormat[i];
		}

		vertexDesc[myVertexFormat.Size() + 0] = { "myWorld", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
		vertexDesc[myVertexFormat.Size() + 1] = { "myWorld", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
		vertexDesc[myVertexFormat.Size() + 2] = { "myWorld", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
		vertexDesc[myVertexFormat.Size() + 3] = { "myWorld", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 };

		InitInputLayout(vertexDesc, size);

		InitVertexBuffer(myVertexBaseData->myStride, D3D11_USAGE_IMMUTABLE, 0);
		InitInstanceBuffer(sizeof(CU::Matrix44<float>), D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		InitIndexBuffer();

		SetupVertexBuffer(myVertexBaseData->myNumberOfVertices
			, myVertexBaseData->myStride, myVertexBaseData->myVertexData);

		SetupInstanceBuffer(1024, sizeof(CU::Matrix44<float>));

		SetupIndexBuffer(myIndexBaseData->myNumberOfIndices, myIndexBaseData->myIndexData);

		myVertexCount = myVertexBaseData->myNumberOfVertices;
	}

	for (int i = 0; i < myChildren.Size(); ++i)
	{
		myChildren[i]->myFileName = myFileName;
		myChildren[i]->Init();
	}

	
	myInited = true;

	if (myEffect->GetFileName() == "Data/Resource/Shader/S_effect_pbl.fx")
	{
		for (int i = 0; i < mySurfaces.Size(); ++i)
		{
			if (mySurfaces[i]->VerifyTextures(myFileName) == false)
			{
				DL_ASSERT(CU::Concatenate("Missing PBL-texture from model: %s", myFileName.c_str()));
			}
		}
	}
}

void Prism::Model::InitCube(const float aWidth, const float aHeight, const float aDepth, CU::Vector4f aColour)
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
	InitInputLayout(vertexDesc, ARRAYSIZE(vertexDesc));


#pragma region Vertices
	CU::GrowingArray<VertexPosColor> vertices;
	vertices.Init(24);

	float halfWidth = aWidth / 2.f;
	float halfHeight = aHeight / 2.f;
	float halfDepth = aDepth / 2.f;

	//0 - 3 (Top)
	vertices.Add({ { -halfWidth, halfHeight, -halfDepth }, aColour });
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

	CU::GrowingArray<int> indices(24);
	//Top
	indices.Add(3);
	indices.Add(1);
	indices.Add(0);

	indices.Add(2);
	indices.Add(1);
	indices.Add(3);

	//Bottom
	indices.Add(6);
	indices.Add(4);
	indices.Add(5);

	indices.Add(7);
	indices.Add(4);
	indices.Add(6);

	//Left
	indices.Add(11);
	indices.Add(9);
	indices.Add(8);

	indices.Add(10);
	indices.Add(9);
	indices.Add(11);

	//Right
	indices.Add(14);
	indices.Add(12);
	indices.Add(13);

	indices.Add(15);
	indices.Add(12);
	indices.Add(14);

	//Front
	indices.Add(19);
	indices.Add(17);
	indices.Add(16);

	indices.Add(18);
	indices.Add(17);
	indices.Add(19);

	//Back
	indices.Add(22);
	indices.Add(20);
	indices.Add(21);

	indices.Add(23);
	indices.Add(20);
	indices.Add(22);

#pragma endregion


	InitVertexBuffer(sizeof(VertexPosColor), D3D11_USAGE_IMMUTABLE, 0);
	InitIndexBuffer();

	SetupVertexBuffer(vertices.Size(), sizeof(VertexPosColor), reinterpret_cast<char*>(&vertices[0]));
	SetupIndexBuffer(indices.Size(), reinterpret_cast<char*>(&indices[0]));

	Surface surf;
	surf.SetEffect(myEffect);
	surf.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	surf.SetVertexStart(0);
	surf.SetVertexCount(vertices.Size());
	surf.SetIndexStart(0);
	surf.SetIndexCount(indices.Size());
	surf.SetTexture("DiffuseTexture", "Data/resources/texture/seafloor.dds", true);

	mySurfaces.Add(new Surface(surf));

	myIsNULLObject = false;
	myInited = true;
}

void Prism::Model::AddChild(Model* aChild)
{
	aChild->myParent = this;
	myChildren.Add(aChild);
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

	for (int i = 0; i < myChildren.Size(); ++i) 
	{
		myChildren[i]->SetEffect(aEffect);
	}
}

void Prism::Model::BeginRender(const CU::GrowingArray<CU::Matrix44<float>>& someOrientations)
{
	if (myIsNULLObject == false)
	{
		float blendFactor[4];
		blendFactor[0] = 0.f;
		blendFactor[1] = 0.f;
		blendFactor[2] = 0.f;
		blendFactor[3] = 0.f;

		myEffect->SetBlendState(NULL, blendFactor);
		BaseModel::BeginRender(someOrientations);
	}
	for (int i = 0; i < myChildren.Size(); ++i)
	{
		myChildren[i]->BeginRender(someOrientations);
	}
}

void Prism::Model::BeginRenderNormal()
{
	if (myIsNULLObject == false)
	{
		float blendFactor[4];
		blendFactor[0] = 0.f;
		blendFactor[1] = 0.f;
		blendFactor[2] = 0.f;
		blendFactor[3] = 0.f;

		myEffect->SetBlendState(NULL, blendFactor);
		BaseModel::BeginRenderNormal();
	}
	for (int i = 0; i < myChildren.Size(); ++i)
	{
		myChildren[i]->BeginRenderNormal();
	}
}

void Prism::Model::Render(const CU::Matrix44<float>& aOrientation)
{
	if (myIsNULLObject == false)
	{

		myEffect->SetWorldMatrix(aOrientation);

		BaseModel::Render();
	}

	for (int i = 0; i < myChildren.Size(); ++i)
	{
		myChildren[i]->Render(myChildTransforms[i] * aOrientation);
	}
}

void Prism::Model::Render(const CU::Matrix44<float>& aOrientation, RenderProcessTarget& aRenderProcessTarget)
{
	aRenderProcessTarget.Render(this, aOrientation);
}

unsigned int Prism::Model::GetIndexCount() const
{
	if (myIsNULLObject == false)
	{
		return mySurfaces[0]->GetIndexCount();
	}
	else
	{
		for (int i = 0; i < myChildren.Size(); ++i)
		{
			return myChildren[i]->GetIndexCount();
		}
	}
}

unsigned int Prism::Model::GetVertexCount() const
{
	if (myIsNULLObject == false)
	{
		return mySurfaces[0]->GetVertexCount();
	}
	else
	{
		for (int i = 0; i < myChildren.Size(); ++i)
		{
			return myChildren[i]->GetVertexCount();
		}
	}
}