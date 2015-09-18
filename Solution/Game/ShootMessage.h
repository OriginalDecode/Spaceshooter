#pragma once
class ShootMessage
{
public:
	ShootMessage(float aSpeed);

	float GetSpeed() const;

private:
	float mySpeed;
};

inline float ShootMessage::GetSpeed() const
{
	return mySpeed;
}