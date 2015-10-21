#pragma once

struct HealthNote
{
	HealthNote(float someHealth, float someMaxHealth);

	float myHealth;
	float myMaxHeatlh;


private:

	void operator=(HealthNote&) = delete;
};

inline HealthNote::HealthNote(float someHealth, float someMaxHealth)
	: myHealth(someHealth)
	, myMaxHeatlh(someMaxHealth)
{
}


