#pragma once

struct EmitterNote
{
	enum class eType
	{
		BULLET
	};

	EmitterNote(eType aType);
	
	eType myType;

private:

	void operator=(EmitterNote&) = delete;

};

inline EmitterNote::EmitterNote(eType aType)
	: myType(aType)
{
}