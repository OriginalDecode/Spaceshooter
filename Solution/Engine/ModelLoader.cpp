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
		: myNonFXBModels(4)
		, myIsRunning(true)
		, myCanAddToLoadArray(true)
		, myCanCopyArray(true)
		, myModelFactory(new FBXFactory())
		, myIsLoading(false)
		, myClearLoadJobs(true)
	{
		myBuffers[0].Init(512);
		myBuffers[1].Init(512);
		myActiveBuffer = 0;
		myInactiveBuffer = 1;
	}

	ModelLoader::~ModelLoader()
	{
		delete myModelFactory;
		myModelFactory = nullptr;
		myNonFXBModels.DeleteAll();
	}

	void ModelLoader::Run()
	{
#ifndef THREADED_LOADING
		return;
#else
		while (myIsRunning == true)
		{
			WaitUntilCopyIsAllowed();
			myCanAddToLoadArray = false;

			if (myBuffers[myInactiveBuffer].Size() == 0)
			{
				myCanAddToLoadArray = true;
				myIsLoading = false;
				std::this_thread::yield();
				continue;
			}

			if (myIsRunning == false || myClearLoadJobs == true)
			{
				myIsLoading = false;
				myClearLoadJobs = false;
				myCanAddToLoadArray = true;
				continue;
			}

			int oldInactive = myInactiveBuffer;
			myInactiveBuffer = myActiveBuffer;
			myActiveBuffer = oldInactive;
			myBuffers[myInactiveBuffer].RemoveAll();

			myCanAddToLoadArray = true;
			myIsLoading = true;

			CU::GrowingArray<LoadData>& loadArray = myBuffers[myActiveBuffer];
			for (int i = 0; i < loadArray.Size(); ++i)
			{
				//check in here aswell to allow early outs so we dont have to wait for 2-3 seconds to quit if
				//we got a big load-array
				if (myIsRunning == false || myClearLoadJobs == true)
				{
					myIsLoading = false;
					myClearLoadJobs = false;
					loadArray.RemoveAll();
					break;
				}

				eLoadType loadType = loadArray[i].myLoadType;

				Model* model = nullptr;
				switch (loadType)
				{
				case Prism::ModelLoader::eLoadType::MODEL:
				{
					
					model = myModelFactory->LoadModel(loadArray[i].myModelPath.c_str(),
						Engine::GetInstance()->GetEffectContainer()->GetEffect(loadArray[i].myEffectPath));
					
					break;
				}
				case Prism::ModelLoader::eLoadType::CUBE:
				{
					model = new Prism::Model();
					model->InitCube(loadArray[i].mySize.x, loadArray[i].mySize.y,
						loadArray[i].mySize.z, loadArray[i].myColor);

					myNonFXBModels.Add(model);
					break;
				}
				default:
					DL_ASSERT("ModelLoader tried to load something that dint have a specified LoadType!!!");
					break;
				}

				if (model == nullptr)
				{
					DL_MESSAGE_BOX("Failed to Load model", "ModelLoader->Error", MB_ICONWARNING);
				}

				loadArray[i].myProxy->SetModel(model);
			}
		}
#endif
	}

	void ModelLoader::Shutdown()
	{
		myIsRunning = false;
	}

	void ModelLoader::ClearLoadJobs()
	{
		myClearLoadJobs = true;
	}

	volatile bool ModelLoader::IsLoading() const
	{
		return myIsLoading;
	}

	ModelProxy* ModelLoader::LoadModel(const std::string& aModelPath, const std::string& aEffectPath)
	{
#ifdef THREADED_LOADING
		WaitUntilAddIsAllowed();

		myCanCopyArray = false;

		ModelProxy* proxy = new ModelProxy();
		proxy->SetModel(nullptr);

		LoadData newData;
		newData.myProxy = proxy;
		newData.myLoadType = eLoadType::MODEL;
		newData.myModelPath = aModelPath;
		newData.myEffectPath = aEffectPath;

		myBuffers[myInactiveBuffer].Add(newData);

		myCanCopyArray = true;

		return proxy;
#else
		ModelProxy* proxy = new ModelProxy();
		
		CU::TimerManager::GetInstance()->StartTimer("LoadModel");

		Model* model = myModelFactory->LoadModel(aModelPath.c_str(),
			Engine::GetInstance()->GetEffectContainer()->GetEffect(aEffectPath));
		model->Init();

		int elapsed = static_cast<int>(
			CU::TimerManager::GetInstance()->StopTimer("LoadModel").GetMilliseconds());
		RESOURCE_LOG("Model \"%s\" took %d ms to load", aModelPath.c_str(), elapsed);

		proxy->SetModel(model);

		return proxy;
#endif
		
	}

	ModelProxy* ModelLoader::LoadCube(float aWidth, float aHeight, float aDepth
		, CU::Vector4f aColour)
	{
#ifdef THREADED_LOADING
		WaitUntilAddIsAllowed();

		myCanCopyArray = false;
		ModelProxy* proxy = new ModelProxy();
		proxy->SetModel(nullptr);

		LoadData newData;
		newData.myProxy = proxy;
		newData.myLoadType = eLoadType::CUBE;
		newData.mySize = { aWidth, aHeight, aDepth };
		newData.myColor = aColour;

		myBuffers[myInactiveBuffer].Add(newData);

		myCanCopyArray = true;

		return proxy;
#else
		ModelProxy* proxy = new ModelProxy();
		Model* model = new Prism::Model();
		model->InitLightCube(aWidth, aHeight, aDepth, aColour);

		proxy->SetModel(model);

		return proxy;
#endif	
	}


	void ModelLoader::WaitUntilCopyIsAllowed()
	{
		while (myCanCopyArray == false)
			; //Should be an empty whileloop!
	}

	void ModelLoader::WaitUntilAddIsAllowed()
	{
		while (myCanAddToLoadArray == false)
			; //Should be an empty whileloop!
	}

}