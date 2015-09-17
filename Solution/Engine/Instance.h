#pragma once

#include <Matrix.h>
#include <StaticArray.h>

namespace Prism
{
	class Camera;
	class Light;
	class Model;

	class Instance
	{
	public:
		Instance(Model& aModel);

		void Render(Camera& aCamera);
		void Render(const CU::Matrix44<float>& aParentMatrix, Camera& aCamera);

		void SetPosition(const CU::Vector3<float>& aPosition);
		CU::Vector3<float>& GetPosition();
		CU::Matrix44<float>& GetOrientation();
		void SetOrientation(const CU::Matrix44<float>& aOrientation);

		void PerformRotationLocal(CU::Matrix44<float>& aRotation);
		void PerformRotationWorld(CU::Matrix44<float>& aRotation);
		void PerformTransformation(CU::Matrix44<float>& aTransformation);

		void UpdateDirectionalLights(const CU::StaticArray<CU::Vector4<float>, 1>& someLightDirections,
			const CU::StaticArray<CU::Vector4<float>, 1>& someLightColors);
		void UpdatePointLights(const CU::StaticArray<CU::Vector4<float>, 3>& someLightPositions,
			const CU::StaticArray<CU::Vector4<float>, 3>& someLightColors,
			const CU::StaticArray<float, 3>& someLightRanges);

	private:
		void operator=(Instance&) = delete;

		Model& myModel;
		CU::Matrix44<float> myOrientation;
		CU::Vector3<float> myPosition;
	};
}