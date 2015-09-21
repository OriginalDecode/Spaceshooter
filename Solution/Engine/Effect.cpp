#include "stdafx.h"

#include <D3DX11async.h>
#include <DL_Debug.h>
#include "Effect.h"
#include "Engine.h"
#include "Texture.h"

bool Prism::Effect::Init(const std::string& aEffectFile)
{

	//Engine::GetInstance()->GetFileWatcher().WatchFile(aEffectFile, std::bind(&Prism::Effect::ReloadShader, this, aEffectFile));

	myEffect = nullptr;
	ReloadShader(aEffectFile);

	return true;
	/*myFileName = aEffectFile;

	HRESULT hr;
	unsigned int shaderFlags = 0;

#if defined (DEBUG) || defined(_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
#endif

	ID3D10Blob* compiledShader = nullptr;
	ID3D10Blob* compilationMsgs = nullptr;

	hr = D3DX11CompileFromFile(aEffectFile.c_str(), 0, 0, 0, "fx_5_0", shaderFlags, 0, 0, &compiledShader
		, &compilationMsgs, 0);
	if (FAILED(hr))
	{
		if (compilationMsgs != nullptr)
		{
			DL_MESSAGE_BOX((char*)compilationMsgs->GetBufferPointer(), "Effect Error", MB_ICONWARNING);
		}
	}
	if (compilationMsgs != nullptr)
	{
		compilationMsgs->Release();
	}

	hr = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL
		, Engine::GetInstance()->GetDevice(), &myEffect);

	if (FAILED(hr))
	{
		DL_MESSAGE_BOX("Cant Create Effect", "Effect Error", MB_ICONWARNING);
		return false;
	}


	myTechnique = myEffect->GetTechniqueByName("Render");
	if (myTechnique->IsValid() == false)
	{
		DL_MESSAGE_BOX("Failed to get Technique", "Effect Error", MB_ICONWARNING);
		return false;
	}

	myWorldMatrixVariable = myEffect->GetVariableByName("World")->AsMatrix();
	if (myWorldMatrixVariable->IsValid() == false)
	{
		DL_MESSAGE_BOX("Failed to get WorldMatrix", "Effect Error", MB_ICONWARNING);
		return false;
	}

	myViewMatrixVariable = myEffect->GetVariableByName("View")->AsMatrix();
	if (myViewMatrixVariable->IsValid() == false)
	{
		DL_MESSAGE_BOX("Failed to get ViewMatrix", "Effect Error", MB_ICONWARNING);
		return false;
	}

	myProjectionMatrixVariable = myEffect->GetVariableByName("Projection")->AsMatrix();
	if (myProjectionMatrixVariable->IsValid() == false)
	{
		DL_MESSAGE_BOX("Failed to get ProjectionMatrix", "Effect Error", MB_ICONWARNING);
		return false;
	}

	myDirectionalLigthColor = myEffect->GetVariableByName("DirectionalLightColor")->AsVector();
	if (myDirectionalLigthColor->IsValid() == false)
	{
		myDirectionalLigthColor = nullptr;;
	}

	myDirectionalLightDirection = myEffect->GetVariableByName("DirectionalLightDir")->AsVector();
	if (myDirectionalLightDirection->IsValid() == false)
	{
		myDirectionalLightDirection = nullptr;
	}

	myPointLightPosition = myEffect->GetVariableByName("PointLightPosition")->AsVector();
	if (myPointLightPosition->IsValid() == false)
	{
		myPointLightPosition = nullptr;;
	}

	myPointLigthColor = myEffect->GetVariableByName("PointLightColor")->AsVector();
	if (myPointLigthColor->IsValid() == false)
	{
		myPointLigthColor = nullptr;
	}

	myPointLightRange = myEffect->GetVariableByName("PointLightRange")->AsScalar();
	if (myPointLightRange->IsValid() == false)
	{
		myPointLightRange = nullptr;
	}

	myTotalTimeVariable = nullptr;
	myTotalTimeVariable = myEffect->GetVariableByName("Time")->AsScalar();
	if (myTotalTimeVariable->IsValid() == false)
	{
		myTotalTimeVariable = nullptr;
	}

	return true;*/
}

void Prism::Effect::UpdateDirectionalLights(
	const CU::StaticArray<Prism::DirectionalLightData, NUMBER_OF_DIRECTIONAL_LIGHTS>& someDirectionalLightData)
{
	if (myDirectionalLightVariable != nullptr)
	{
		myDirectionalLightVariable->SetRawValue(&someDirectionalLightData[0], 0, 
			sizeof(DirectionalLightData) * NUMBER_OF_DIRECTIONAL_LIGHTS);
	}
}

void Prism::Effect::UpdatePointLights(const CU::StaticArray<PointLightData, NUMBER_OF_POINT_LIGHTS>& somePointLightData)
{
	if (myPointLightVariable != nullptr)
	{
		myPointLightVariable->SetRawValue(&somePointLightData[0], 0, sizeof(PointLightData) * NUMBER_OF_POINT_LIGHTS);
	}
}

void Prism::Effect::UpdateSpotLights(const CU::StaticArray<SpotLightData, NUMBER_OF_SPOT_LIGHTS>& someSpotLightData)
{
	if (mySpotLightVariable != nullptr)
	{
		mySpotLightVariable->SetRawValue(&someSpotLightData[0], 0, sizeof(SpotLightData) * NUMBER_OF_SPOT_LIGHTS);
	}
}

void Prism::Effect::UpdateTime(const float aDeltaTime)
{
	if (myTotalTimeVariable != nullptr)
	{
		float newTime = 0.f;
		myTotalTimeVariable->GetFloat(&newTime);
		newTime += aDeltaTime;
		myTotalTimeVariable->SetFloat(newTime);
	}
}


void Prism::Effect::ReloadShader(const std::string& aFile)
{
	//if (myEffect != nullptr)
	//{
	//	myEffect->Release();
	//}

	myFileName = aFile;

	HRESULT hr;
	unsigned int shaderFlags = 0;

#if defined (DEBUG) || defined(_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
#endif

	ID3D10Blob* compiledShader = nullptr;
	ID3D10Blob* compilationMsgs = nullptr;

	hr = D3DX11CompileFromFile(myFileName.c_str(), 0, 0, 0, "fx_5_0", shaderFlags, 0, 0, &compiledShader
		, &compilationMsgs, 0);
	if (FAILED(hr))
	{
		if (compilationMsgs != nullptr)
		{
			DL_MESSAGE_BOX((char*)compilationMsgs->GetBufferPointer(), "Effect Error", MB_ICONWARNING);
		}
	}
	if (compilationMsgs != nullptr)
	{
		compilationMsgs->Release();
	}

	hr = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL
		, Engine::GetInstance()->GetDevice(), &myEffect);

	if (FAILED(hr))
	{
		DL_MESSAGE_BOX("Cant Create Effect", "Effect Error", MB_ICONWARNING);
		return;
	}


	compiledShader->Release();


	myTechnique = myEffect->GetTechniqueByName("Render");
	if (myTechnique->IsValid() == false)
	{
		DL_MESSAGE_BOX("Failed to get Technique", "Effect Error", MB_ICONWARNING);
		return;
	}

	myWorldMatrixVariable = myEffect->GetVariableByName("World")->AsMatrix();
	if (myWorldMatrixVariable->IsValid() == false)
	{
		DL_MESSAGE_BOX("Failed to get WorldMatrix", "Effect Error", MB_ICONWARNING);
		return;
	}

	myViewMatrixVariable = myEffect->GetVariableByName("View")->AsMatrix();
	if (myViewMatrixVariable->IsValid() == false)
	{
		DL_MESSAGE_BOX("Failed to get ViewMatrix", "Effect Error", MB_ICONWARNING);
		return;
	}

	myProjectionMatrixVariable = myEffect->GetVariableByName("Projection")->AsMatrix();
	if (myProjectionMatrixVariable->IsValid() == false)
	{
		DL_MESSAGE_BOX("Failed to get ProjectionMatrix", "Effect Error", MB_ICONWARNING);
		return;
	}

	myTotalTimeVariable = nullptr;
	myTotalTimeVariable = myEffect->GetVariableByName("Time")->AsScalar();
	if (myTotalTimeVariable->IsValid() == false)
	{
		myTotalTimeVariable = nullptr;
	}



	myDirectionalLightVariable = myEffect->GetVariableByName("DirectionalLights");
	if (myDirectionalLightVariable->IsValid() == false)
	{
		myDirectionalLightVariable = nullptr;
	}

	myPointLightVariable = myEffect->GetVariableByName("PointLights");
	if (myPointLightVariable->IsValid() == false)
	{
		myPointLightVariable = nullptr;
	}

	mySpotLightVariable = myEffect->GetVariableByName("SpotLights");
	if (mySpotLightVariable->IsValid() == false)
	{
		mySpotLightVariable = nullptr;
	}
}