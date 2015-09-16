#include "stdafx.h"

#include "Effect.h"
#include <D3DX11async.h>
#include <DL_Debug.h>
#include "Engine.h"

Effect::Effect()
{
}


Effect::~Effect()
{
}

bool Effect::Init(const std::string& aEffectFile)
{
	myFileName = aEffectFile;

	HRESULT hr;
	unsigned int shaderFlags = 0;

#if defined (DEBUG) || defined(_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
#endif

	ID3D10Blob* compiledShader = nullptr;
	ID3D10Blob* compilationMsgs = nullptr;

	hr = D3DX11CompileFromFile(aEffectFile.c_str(), 0, 0, 0, "fx_5_0", shaderFlags, 0, 0, &compiledShader, &compilationMsgs, 0);
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

	hr = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL, Engine::GetInstance()->GetDevice(), &myEffect);

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

	return true;
}

void Effect::UpdateDirectionalLight(CU::StaticArray<CU::Vector4<float>, 1> someDirs, CU::StaticArray<CU::Vector4<float>, 1> someColors)
{
	if (myDirectionalLightDirection != nullptr && myDirectionalLigthColor != nullptr)
	{
		myDirectionalLightDirection->SetFloatVectorArray(reinterpret_cast<float*>(&someDirs[0]), 0, 1);
		myDirectionalLigthColor->SetFloatVectorArray(reinterpret_cast<float*>(&someColors[0]), 0, 1);
	}
}

void Effect::UpdatePointLight(CU::StaticArray<CU::Vector4<float>, 3> somePositions, CU::StaticArray<CU::Vector4<float>, 3> someColors, CU::StaticArray<float, 3> someRanges)
{
	if (myPointLightPosition != nullptr && myPointLigthColor != nullptr)
	{
		myPointLightPosition->SetFloatVectorArray(reinterpret_cast<float*>(&somePositions[0]), 0, 3);
		myPointLigthColor->SetFloatVectorArray(reinterpret_cast<float*>(&someColors[0]), 0, 3);
		myPointLightRange->SetFloatArray(reinterpret_cast<float*>(&someRanges[0]), 0, 3);
	}
}

void Effect::UpdateTime(const float aDeltaTime)
{
	if (myTotalTimeVariable != nullptr)
	{
		float newTime = 0.f;
		myTotalTimeVariable->GetFloat(&newTime);
		newTime += aDeltaTime;
		myTotalTimeVariable->SetFloat(newTime);
	}
}