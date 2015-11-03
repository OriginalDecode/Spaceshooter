#include "stdafx.h"

#include "Camera.h"
#include <d3dx11effect.h>
#include "Effect.h"
#include "Model.h"
#include "RenderProcessTarget.h"


void Prism::RenderProcessTarget::Render(Model* aModel, const CU::Matrix44<float>& aOrientation)
{
	unsigned int exists = myData.count(aModel);

	if (exists != 0)
	{
		myData[aModel].myOrientations.Add(aOrientation);
	}
	else
	{
		myData[aModel] = ModelMeshInstancingData(aModel, aOrientation);
	}
}

void Prism::RenderProcessTarget::RenderNormal()
{
	Effect* oldEffect = nullptr;

	for (auto it = myData.begin(); it != myData.end(); ++it)
	{
		Model& model = *it->second.myModel;
		Effect* newEffect = model.GetEffect();

		if (newEffect != oldEffect)
		{
			newEffect->SetViewMatrix(CU::InverseSimple(myCamera->GetOrientation()));
			newEffect->SetProjectionMatrix(myCamera->GetProjection());
			newEffect->SetScaleVector({ 100.f, 100.f, 100.f });
		}

		model.BeginRender();

		for (int i = 0; i < it->second.myOrientations.Size(); ++i)
		{
			newEffect->SetWorldMatrix(it->second.myOrientations[i]);



			ID3DX11EffectTechnique* tech = nullptr;
			D3DX11_TECHNIQUE_DESC techDesc;


			tech = newEffect->GetEffect()->GetTechniqueByName("Render_Emissive");


			DL_ASSERT_EXP(tech != nullptr, "Technique is nullptr");
			DL_ASSERT_EXP(tech->IsValid() != false, "Technique is not valid");

			tech->GetDesc(&techDesc);
			for (UINT i = 0; i < techDesc.Passes; ++i)
			{
				tech->GetPassByIndex(i)->Apply(0, Engine::GetInstance()->GetContex());
				Engine::GetInstance()->GetContex()->DrawIndexed(model.GetIndexCount()
					, 0, 0);
			}
		}
	}

	myData.clear();
}

void Prism::RenderProcessTarget::RenderNormalOld()
{
	Effect* oldEffect = nullptr;

	for (auto it = myData.begin(); it != myData.end(); ++it)
	{
		Model& model = *it->second.myModel;
		Effect* newEffect = model.GetEffect();

		//if (newEffect != oldEffect)
		{
		}

		for (int i = 0; i < it->second.myOrientations.Size(); ++i)
		{
			newEffect->SetViewMatrix(CU::InverseSimple(myCamera->GetOrientation()));
			newEffect->SetProjectionMatrix(myCamera->GetProjection());
			newEffect->SetScaleVector({ 100.f, 100.f, 100.f });
			model.BeginRender();
			newEffect->SetWorldMatrix(it->second.myOrientations[i]);



			ID3DX11EffectTechnique* tech = nullptr;
			D3DX11_TECHNIQUE_DESC techDesc;


			tech = newEffect->GetEffect()->GetTechniqueByName("Render_Emissive");


			DL_ASSERT_EXP(tech != nullptr, "Technique is nullptr");
			DL_ASSERT_EXP(tech->IsValid() != false, "Technique is not valid");

			tech->GetDesc(&techDesc);
			for (UINT i = 0; i < techDesc.Passes; ++i)
			{
				tech->GetPassByIndex(i)->Apply(0, Engine::GetInstance()->GetContex());
				Engine::GetInstance()->GetContex()->DrawIndexed(model.GetIndexCount()
					, 0, 0);
			}
		}
	}

	myData.clear();
}