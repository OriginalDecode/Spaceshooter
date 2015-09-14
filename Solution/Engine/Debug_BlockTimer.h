#pragma once

class Debug_BlockTimer
{
public:
	Debug_BlockTimer(const char* aName);
	~Debug_BlockTimer();

private:
	const char* myName;
	unsigned long long myStartTime;
};

