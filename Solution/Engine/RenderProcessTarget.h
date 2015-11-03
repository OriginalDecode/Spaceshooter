#pragma once
#include <GrowingArray.h>
#include <Matrix.h>
#include <unordered_map>

namespace Prism
{
	class Camera;

	class RenderProcessTarget
	{
	public:
		struct ModelMeshInstancingData
		{
			ModelMeshInstancingData() {};
			ModelMeshInstancingData(Model* aModel, const CU::Matrix44<float>& aOrientation) : myModel(aModel), myOrientations(16) { myOrientations.Add(aOrientation); }
			Model* myModel;
			CU::GrowingArray<CU::Matrix44<float>> myOrientations;
		};


		void SetCamera(Camera* aCamera);
		void Render(Model* aModel, const CU::Matrix44<float>& aOrientation);
		void RenderNormal();
		void RenderNormalOld();

	private:
		void operator=(RenderProcessTarget&) = delete;
		Camera* myCamera;
		std::unordered_map<Model*, ModelMeshInstancingData> myData;
	};
}


inline void Prism::RenderProcessTarget::SetCamera(Camera* aCamera)
{
	myCamera = aCamera;
}