#pragma once
#include <Matrix.h>

	
namespace CommonUtilities
{
	class InputWrapper;
}

class Player
{
public:
	Player(CU::InputWrapper& aInputWrapper);
	//N�r spelaren skapas s� skall dennes matrix skickas in till kameran som en referens, s� att kameran r�r sig som splearen.

	CU::Matrix44f& GetOrientation();
	
	void Update(float aDeltaTime);

	


private:
	void operator= (const Player&) = delete;
	void MoveForward(float aDistance);
	void MoveRight(float aDistance);

	void RotateX(float aRadian);
	void RotateY(float aRadian);
	void RotateZ(float aRadian);

	CU::Matrix44f myOrientation;
	CU::Vector3f myPosition;

	CU::Vector2<float> myCursorPosition;

	CU::InputWrapper& myInputWrapper;
};