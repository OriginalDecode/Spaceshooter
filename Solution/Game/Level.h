#pragma once

namespace Prism
{
	class Camera;
	class DirectionalLight;
	class Model;
	class PointLight;
	class Scene;
	class Text;
	class Instance;
};

class BulletManager;
class CU::InputWrapper;

class Entity;

class Level
{
public:
	Level(const std::string& aFileName, CU::InputWrapper* aInputWrapper, BulletManager* aBulletManager, bool aShouldTestXML);
	~Level();

	void SetSkySphere(const std::string& aModelFilePath, const std::string& aEffectFileName);

	void Render();
	void LogicUpdate(float aDeltaTime);

	void OnResize(int aWidth, int aHeigth);
	bool CheckCollision();

	inline void SetShowLightCube(bool aBool);
	inline bool GetShowLightCube() const;

	inline void SetRenderStuff(bool aBool);
	inline bool GetRenderStuff() const;

private:
	void ReadXML(const std::string& aFile);
	Prism::Instance* mySkySphere;

	Prism::Scene* myScene;
	Prism::Camera* myCamera;
	Prism::DirectionalLight* myLight;
	Prism::PointLight* myPointLight;
	CU::Matrix44<float> myWorldMatrix;
	CU::GrowingArray<Entity*> myEntities;
	Entity* myPlayer;
	Entity* myCockPit;

	CU::InputWrapper* myInputWrapper;


	BulletManager* myBulletManager;

	bool myRenderStuff;
	bool myShowPointLightCube;
};


void Level::SetShowLightCube(bool aBool)
{
	myShowPointLightCube = aBool;
}

inline bool Level::GetShowLightCube() const
{
	return myShowPointLightCube;
}

inline void Level::SetRenderStuff(bool aBool)
{
	myRenderStuff = aBool;
}

inline bool Level::GetRenderStuff() const
{
	return myRenderStuff;
}