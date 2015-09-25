#include "stdafx.h"

#include "Engine.h"
#include "EffectContainer.h"
#include "Model.h"
#include "ModelLoader.h"
#include "ModelProxy.h"
#include "FBXFactory.h"
#include <TimerManager.h>

namespace Prism
{
	ModelLoader::ModelLoader()
	{
		myModelsToLoad.Init(4);

		myIsRunning = true;
		myCanAddToLoadArray = true;
		myCanCopyArray = true;

		myModelFactory = new FBXFactory();
	}

	void ModelLoader::Run()
	{
		while (myIsRunning == true)
		{
			CU::GrowingArray<LoadData> loadArray;
			if (myCanCopyArray == true)
			{
				myCanAddToLoadArray = false;
				loadArray = myModelsToLoad;
				myModelsToLoad.RemoveAll();
				myCanAddToLoadArray = true;
			}
			

			if (loadArray.Size() == 0)
			{
				Sleep(500);
			}
			else
			{
				for (int i = 0; i < loadArray.Size(); ++i)
				{
					eLoadType loadType = loadArray[i].myLoadType;

					Model* model = nullptr;
					switch (loadType)
					{
					case Prism::ModelLoader::eLoadType::MODEL:
					{
						CU::TimerManager::GetInstance()->StartTimer("LoadModel");

						model = myModelFactory->LoadModel(loadArray[i].myModelPath.c_str(),
							Engine::GetInstance()->GetEffectContainer()->GetEffect(loadArray[i].myEffectPath));
						model->Init();

						int elapsed = static_cast<int>(
							CU::TimerManager::GetInstance()->StopTimer("LoadModel").GetMilliseconds());
						RESOURCE_LOG("Model \"%s\" took %d ms to load", loadArray[i].myModelPath.c_str(), elapsed);
						break;
					}
					case Prism::ModelLoader::eLoadType::POLYGON:
					{
						model = new Prism::Model();
						model->InitPolygon();
						break;
					}
					case Prism::ModelLoader::eLoadType::CUBE:
					{
						model = new Prism::Model();
						model->InitCube(loadArray[i].mySize.x, loadArray[i].mySize.y, loadArray[i].mySize.z);
						break;
					}
					case Prism::ModelLoader::eLoadType::LIGHT_CUBE:
					{
						model = new Prism::Model();
						model->InitLightCube(loadArray[i].mySize.x, loadArray[i].mySize.y,
							loadArray[i].mySize.z, loadArray[i].myColor);
						break;
					}
					case Prism::ModelLoader::eLoadType::GEOMETRY:
					{
						model = new Prism::Model();
						model->InitGeometry(loadArray[i].myMeshData);
						break;
					}
					default:
						DL_ASSERT("ModelLoader tried to load something that dint have a specified LoadType!!!");
						break;
					}

					loadArray[i].myProxy->SetModel(model);
				}
			}
		}
	}

	void ModelLoader::Shutdown()
	{
		myIsRunning = false;
	}

	ModelProxy* ModelLoader::LoadModel(const std::string& aModelPath, const std::string& aEffectPath)
	{
		while (myCanAddToLoadArray == false)
			; //Should be an empty whileloop!

		myCanCopyArray = false;

		ModelProxy* proxy = new ModelProxy();

		LoadData newData;
		newData.myProxy = proxy;
		newData.myLoadType = eLoadType::MODEL;
		newData.myModelPath = aModelPath;
		newData.myEffectPath = aEffectPath;

		myModelsToLoad.Add(newData);

		myCanCopyArray = true;

		return proxy;
	}

	ModelProxy* ModelLoader::LoadPolygon()
	{
		while (myCanAddToLoadArray == false)
			; //Should be an empty whileloop!

		myCanCopyArray = false;
		ModelProxy* proxy = new ModelProxy();

		LoadData newData;
		newData.myProxy = proxy;
		newData.myLoadType = eLoadType::POLYGON;


		myModelsToLoad.Add(newData);

		myCanCopyArray = true;

		return proxy;
	}

	ModelProxy* ModelLoader::LoadCube(float aWidth, float aHeight, float aDepth)
	{
		while (myCanAddToLoadArray == false)
			; //Should be an empty whileloop!

		myCanCopyArray = false;
		ModelProxy* proxy = new ModelProxy();

		LoadData newData;
		newData.myProxy = proxy;
		newData.myLoadType = eLoadType::CUBE;
		newData.mySize = { aWidth, aHeight, aDepth };

		myModelsToLoad.Add(newData);

		myCanCopyArray = true;

		return proxy;
	}

	ModelProxy* ModelLoader::LoadLightCube(float aWidth, float aHeight, float aDepth
		, CU::Vector4f aColour)
	{
		while (myCanAddToLoadArray == false)
			; //Should be an empty whileloop!

		myCanCopyArray = false;
		ModelProxy* proxy = new ModelProxy();

		LoadData newData;
		newData.myProxy = proxy;
		newData.myLoadType = eLoadType::LIGHT_CUBE;
		newData.mySize = { aWidth, aHeight, aDepth };
		newData.myColor = aColour;

		myModelsToLoad.Add(newData);

		myCanCopyArray = true;

		return proxy;
	}

	ModelProxy* ModelLoader::LoadGeometry(const MeshData& aMeshData)
	{
		while (myCanAddToLoadArray == false)
			; //Should be an empty whileloop!

		myCanCopyArray = false;
		ModelProxy* proxy = new ModelProxy();

		LoadData newData;
		newData.myProxy = proxy;
		newData.myLoadType = eLoadType::GEOMETRY;
		newData.myMeshData = aMeshData;

		myModelsToLoad.Add(newData);

		myCanCopyArray = true;

		return proxy;
	}
}