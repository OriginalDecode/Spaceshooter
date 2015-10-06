#pragma once

class InputNote
{
public:
	InputNote(const unsigned int& aKey);

	const unsigned int& GetKey() const;

private:
	unsigned int myKey;
};

inline InputNote::InputNote(const unsigned int& aKey)
	: myKey(aKey)
{
}

inline const unsigned int& InputNote::GetKey() const
{
	return myKey;
}