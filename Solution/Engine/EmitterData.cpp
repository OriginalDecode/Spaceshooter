#include "stdafx.h"

#include <d3dx11effect.h>

#include "EmitterData.h"
#include <XMLReader.h>

namespace Prism
{
	void EmitterData::LoadDataFile(const char* aFilePath)
	{
		XMLReader read;
		myFileName = aFilePath;

		read.OpenDocument(aFilePath);
		tinyxml2::XMLElement* emitter = read.ForceFindFirstChild("Emitter");
		tinyxml2::XMLElement* element;
		element = read.ForceFindFirstChild(emitter, "Shader");
		read.ReadAttribute(element, "filepath", myEffectName);

		element = read.ForceFindFirstChild(emitter, "Texture");
		read.ReadAttribute(element, "filepath", myTextureName);

		element = read.ForceFindFirstChild(emitter, "EmitterSize");
		read.ReadAttribute(element, "x", myEmitterSize.x);
		read.ReadAttribute(element, "y", myEmitterSize.y);
		read.ReadAttribute(element, "z", myEmitterSize.z);


		element = read.ForceFindFirstChild(emitter, "ParticleRotationDelta");
		read.ReadAttribute(element, "x", myRotationDelta.x);
		read.ReadAttribute(element, "y", myRotationDelta.y);
		read.ReadAttribute(element, "z", myRotationDelta.z);


		element = read.ForceFindFirstChild(emitter, "ParticleMaxRotation");
		read.ReadAttribute(element, "x", myMaxRotation.x);
		read.ReadAttribute(element, "y", myMaxRotation.y);
		read.ReadAttribute(element, "z", myMaxRotation.z);


		element = read.ForceFindFirstChild(emitter, "ParticleMinRotation");
		read.ReadAttribute(element, "x", myMinRotation.x);
		read.ReadAttribute(element, "y", myMinRotation.y);
		read.ReadAttribute(element, "z", myMinRotation.z);


		element = read.ForceFindFirstChild(emitter, "ParticleMaxVelocity");
		read.ReadAttribute(element, "x", myMaxVelocity.x);
		read.ReadAttribute(element, "y", myMaxVelocity.y);
		read.ReadAttribute(element, "z", myMaxVelocity.z);


		element = read.ForceFindFirstChild(emitter, "ParticleMinVelocity");
		read.ReadAttribute(element, "x", myMinVelocity.x);
		read.ReadAttribute(element, "y", myMinVelocity.y);
		read.ReadAttribute(element, "z", myMinVelocity.z);


		element = read.ForceFindFirstChild(emitter, "EmissionVeloctiyDelta");
		read.ReadAttribute(element, "x", myEmissionVelocityDelta.x);
		read.ReadAttribute(element, "y", myEmissionVelocityDelta.y);
		read.ReadAttribute(element, "z", myEmissionVelocityDelta.z);


		element = read.ForceFindFirstChild(emitter, "ParticleStartColor");
		read.ReadAttribute(element, "r", myData.myStartColor.r);
		read.ReadAttribute(element, "g", myData.myStartColor.g);
		read.ReadAttribute(element, "b", myData.myStartColor.b);

		element = read.ForceFindFirstChild(emitter, "ParticleEndColor");
		read.ReadAttribute(element, "r", myData.myEndColor.r);
		read.ReadAttribute(element, "g", myData.myEndColor.g);
		read.ReadAttribute(element, "b", myData.myEndColor.b);


		element = read.ForceFindFirstChild(emitter, "ParticleLifeTime");
		read.ReadAttribute(element, "value", myParticlesLifeTime);

		element = read.ForceFindFirstChild(emitter, "EmissionRate");
		read.ReadAttribute(element, "value", myEmissionRate);

		element = read.ForceFindFirstChild(emitter, "ParticlesPerEmitt");
		read.ReadAttribute(element, "value", myParticlesPerEmitt);

		element = read.ForceFindFirstChild(emitter, "EmissionRateDelta");
		read.ReadAttribute(element, "value", myEmissionRateDelta);


		element = read.ForceFindFirstChild(emitter, "ParticleMaxScale");
		read.ReadAttribute(element, "value", myMaxScale);
		element = read.ForceFindFirstChild(emitter, "ParticleMinScale");
		read.ReadAttribute(element, "value", myMinScale);


		/*element = read.ForceFindFirstChild(emitter, "ParticleAlphaDelta");
		read.ReadAttribute(element, "value", myData.myAlphaDelta);
		element = read.ForceFindFirstChild(emitter, "ParticleSizeDelta");
		read.ReadAttribute(element, "value", myData.mySizeDelta);

		element = read.ForceFindFirstChild(emitter, "ParticleAlphaStart");
		read.ReadAttribute(element, "value", myData.myStartAlpha);*/

		read.CloseDocument();

		myData.myStartColor.r = (myData.myStartColor.r / 255.f);
		myData.myStartColor.g = (myData.myStartColor.g / 255.f);
		myData.myStartColor.b = (myData.myStartColor.b / 255.f);

		myData.myEndColor.r = (myData.myEndColor.r / 255.f);
		myData.myEndColor.g = (myData.myEndColor.g / 255.f);
		myData.myEndColor.b = (myData.myEndColor.b / 255.f);

		myData.myMaxStartSize = myMaxScale;
		myData.myMinStartSize = myMinScale;
		
		myData.myMaxVelocity = myMaxVelocity;
		myData.myMinVelocity = myMinVelocity;

		myData.myLifeTime = myParticlesLifeTime;

		myTexture = Engine::GetInstance()->GetTextureContainer()->GetTexture(myTextureName.c_str());
		myEffect = Engine::GetInstance()->GetEffectContainer()->GetEffect(myEffectName.c_str());
		
		CreateInputLayout();
	}

	void EmitterData::CreateInputLayout()
	{
		HRESULT hr;

		D3DX11_PASS_DESC passDesc;
		hr = myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
		DL_ASSERT_EXP(!FAILED(hr), "[EmitterData](CreateInputLayout) : Failed to get Pass Description!");

		const D3D11_INPUT_ELEMENT_DESC VertexParticleLayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "ALPHA", 0, DXGI_FORMAT_R32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "SIZE", 0, DXGI_FORMAT_R32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TIME", 0, DXGI_FORMAT_R32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		UINT size = ARRAYSIZE(VertexParticleLayout);

		hr = Engine::GetInstance()->GetDevice()->CreateInputLayout(VertexParticleLayout
			, size
			, passDesc.pIAInputSignature
			, passDesc.IAInputSignatureSize
			, &myInputLayout);
		DL_ASSERT_EXP(!FAILED(hr), "[EmitterData](CreateInputLayout) : Failed to Create InputLayout!");
	}
}