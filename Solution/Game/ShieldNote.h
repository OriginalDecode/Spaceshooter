#pragma once

struct ShieldNote
{
	ShieldNote(int someShieldStrength, int someMaxShieldStrength);

	int myShieldStrength;
	int myMaxShieldStrength;


private:

	void operator=(ShieldNote&) = delete;
};

inline ShieldNote::ShieldNote(int someShieldStrength, int someMaxShieldStrength)
	: myShieldStrength(someShieldStrength)
	, myMaxShieldStrength(someMaxShieldStrength)
{
}