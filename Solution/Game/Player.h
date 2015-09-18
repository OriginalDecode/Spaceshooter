#pragma once
#include <Matrix.h>

	
namespace CommonUtilities
{
	class InputWrapper;
}

namespace Prism 
{
	class Camera;
}

class Entity;

class Player
{
public:
	Player(CU::InputWrapper& aInputWrapper);
	//N�r spelaren skapas s� skall dennes matrix skickas in till kameran som en referens, s� att kameran r�r sig som splearen.

	CU::Matrix44f& GetOrientation();
	
	void Update(float aDeltaTime);

	void Render(Prism::Camera* aCamera);

private:
	void operator= (const Player&) = delete;
	void MoveForward(float aDistance);
	void MoveRight(float aDistance);

	void RotateX(float aRadian);
	void RotateY(float aRadian);
	void RotateZ(float aRadian);

	void ShootTest();

	CU::Matrix44f myOrientation;
	CU::Vector3f myPosition;

	CU::Vector2<float> myCursorPosition;

	CU::InputWrapper& myInputWrapper;

	// test
	Entity* myTestBullet;
	bool myIsShooting;
};