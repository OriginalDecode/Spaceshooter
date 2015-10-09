#pragma once

#include "Defines.h"
#include "LightStructs.h"
#include <Matrix.h>
#include <StaticArray.h>

namespace Prism
{
	class Camera;
	class Light;
	class ModelProxy;
	enum class eOctreeType;

	class Instance
	{
	public:

		Instance(ModelProxy& aModel, eOctreeType anOctreeType);
		~Instance();

		void Render(Camera& aCamera);
		void Render(const CU::Matrix44<float>& aParentMatrix, Camera& aCamera);

		void SetPosition(const CU::Vector3<float>& aPosition);
		CU::Vector3<float> GetPosition();
		CU::Matrix44<float>& GetOrientation();
		void SetOrientation(const CU::Matrix44<float>& aOrientation);

		void SetEffect(const std::string& aEffectFile);

		void SetScale(const CU::Vector3<float>& aScaleVector);

		void PerformRotationLocal(CU::Matrix44<float>& aRotation);
		void PerformRotationWorld(CU::Matrix44<float>& aRotation);
		void PerformTransformation(CU::Matrix44<float>& aTransformation);

		void UpdateDirectionalLights(
			const CU::StaticArray<DirectionalLightData, NUMBER_OF_DIRECTIONAL_LIGHTS>& someDirectionalLightData);
		void UpdatePointLights(const CU::StaticArray<PointLightData, NUMBER_OF_POINT_LIGHTS>& somePointLightData);
		void UpdateSpotLights(const CU::StaticArray<SpotLightData, NUMBER_OF_SPOT_LIGHTS>& someSpotLightData);

		void SetOrientationPointer(CU::Matrix44<float>& aOrientation);

		float GetRadius() const;
		eOctreeType GetOctreeType() const;

	private:
		void operator=(Instance&) = delete;

		ModelProxy& myProxy;
		const eOctreeType myOctreeType;
		CU::Matrix44<float> myOrientation;
		CU::Matrix44<float>* myOrientationPointer;
		CU::Vector3<float> myScale;

		float myRadius;
	};

	inline float Instance::GetRadius() const
	{
		return myRadius;
	}

	inline eOctreeType Instance::GetOctreeType() const
	{
		return myOctreeType;
	}
}