#pragma once
#include <string>
#include <unordered_map>

class Event;

class EventManager
{
public:
	EventManager(const std::string& aXmlPath);
	~EventManager();

private:
	std::unordered_map<std::string, Event*> myEvents;
};

