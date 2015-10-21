#pragma once
#include <string>

class Reader
{
public:
	Reader();
	~Reader();
	void ReadFile(const std::string& aFilePath);
};

