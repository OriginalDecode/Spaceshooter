#pragma once

class InputMessage
{
public:
	InputMessage(const unsigned int& aKey);

	const unsigned int& GetKey() const;

private:
	unsigned int myKey;
};

inline const unsigned int& InputMessage::GetKey() const
{
	return myKey;
}