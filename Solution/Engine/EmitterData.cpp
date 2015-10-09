#include "stdafx.h"

#include <d3dx11effect.h>

#include "EmitterData.h"

namespace Prism
{
	void EmitterData::LoadDataFile(const char* aFilePath)
	{

		//Insert data here from XML


		myTexture = Engine::GetInstance()->GetTextureContainer()->GetTexture(myTextureName.c_str());
		myEffect = Engine::GetInstance()->GetEffectContainer()->GetEffect(myEffectName.c_str());
	}

	void EmitterData::CreateInputLayout()
	{
		HRESULT hr = S_OK;

		D3DX11_PASS_DESC passDesc;
		myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);

		DL_ASSERT_EXP(FAILED(hr), "[EmitterData](CreateInputLayout) : Failed to get pass description!");

		const D3D11_INPUT_ELEMENT_DESC VertexParticleLayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "ALPHA", 0, DXGI_FORMAT_R32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "SIZE", 0, DXGI_FORMAT_R32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TIME", 0, DXGI_FORMAT_R32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "RESERVED", 0, DXGI_FORMAT_R32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		UINT arrSize = ARRAYSIZE(VertexParticleLayout);

		hr = Engine::GetInstance()->GetDevice()->CreateInputLayout(VertexParticleLayout
			, arrSize
			, passDesc.pIAInputSignature
			, passDesc.IAInputSignatureSize
			, &myInputLayout);

		DL_ASSERT_EXP(FAILED(hr), "[Emitterdata](CreateInputLayout) : Failed to Create InputLayout!");
	}
}