#pragma once

struct InputNote
{
	InputNote(unsigned int aKey);

	const unsigned int myKey;
};

inline InputNote::InputNote(unsigned int aKey)
	: myKey(aKey)
{
}