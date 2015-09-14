#pragma once
#include <vector>

class Model;
class FBXLoader;
struct ModelData;
class FbxModelData;
class Effect;

class FBXFactory
{
public:
	FBXFactory();
	~FBXFactory();

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