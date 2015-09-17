#pragma once
#include <vector>

struct ModelData;

class FBXLoader;
class FbxModelData;

namespace Prism
{
class Model;
class Effect;
	class FBXFactory
	{
	public:
		FBXFactory();

		Model* LoadModel(const char* aFilePath, Effect* aEffect);
	private:
		void FillData(ModelData* someData, Model* outData, Effect* aEffect);
		Model* CreateModel(FbxModelData* someModelData, Effect* aEffect);

		FBXLoader *myLoader;

		struct FBXData
		{
			FbxModelData* myData;
			std::string myPath;
		};
		std::vector<FBXData*> myFBXData;
	};
}