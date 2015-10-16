#pragma once

#include <GrowingArray.h>
#include <atomic>

namespace Prism
{
	class Model;
	class ModelProxy;
	class FBXFactory;

	class ModelLoader
	{
	public:
		ModelLoader();
		~ModelLoader();

		void Run();
		void Shutdown();
		void ClearLoadJobs();

		volatile bool IsLoading() const;

		ModelProxy* LoadModel(const std::string& aModelPath, const std::string& aEffectPath);
		ModelProxy* LoadPolygon();
		ModelProxy* LoadCube(float aWidth = 1.f, float aHeight = 1.f, float aDepth = 1.f);
		ModelProxy* LoadLightCube(float aWidth = 1.f, float aHeight = 1.f, float aDepth = 1.f
			, CU::Vector4f aColour = { 1.f, 1.f, 1.f, 1.f });

	private:
		enum class eLoadType
		{
			MODEL,
			POLYGON,
			CUBE,
			LIGHT_CUBE,
			GEOMETRY,
		};
		struct LoadData
		{
			ModelProxy* myProxy;
			eLoadType myLoadType;
			std::string myModelPath = "";
			std::string myEffectPath = "";
			CU::Vector3<float> mySize;
			CU::Vector4<float> myColor;
		};

		void WaitUntilCopyIsAllowed();
		void WaitUntilAddIsAllowed();

		CU::GrowingArray<LoadData> myBuffers[2];
		int myActiveBuffer;
		int myInactiveBuffer;
		volatile bool myCanAddToLoadArray;
		volatile bool myCanCopyArray;
		volatile bool myIsRunning;
		volatile bool myIsLoading;
		volatile bool myClearLoadJobs;

		FBXFactory* myModelFactory;
		CU::GrowingArray<Model*> myNonFXBModels;
	};
}