#pragma once

struct EmitterNote
{
	enum class eType
	{
		BULLET
	};

	EmitterNote(eType aType);
	EmitterNote(eType aType, bool aIsActive);

	
	eType myType;
	bool myIsActive;

private:

	void operator=(EmitterNote&) = delete;

};

inline EmitterNote::EmitterNote(eType aType)
	: myType(aType)
{
}

inline EmitterNote::EmitterNote(eType aType, bool aIsActive)
: myType(aType)
, myIsActive(aIsActive)
{
}