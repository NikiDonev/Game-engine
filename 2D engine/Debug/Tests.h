#pragma once
#include <functional>
#include <string>
#include <vector>

struct EngineContext;

struct Test{
	std::string name;
	std::function<void(EngineContext&)> Init;
	std::function<void(EngineContext&)> Update;
	std::function<void()> Shutdown;
};

inline std::vector<Test> tests;

void RegisterTests();