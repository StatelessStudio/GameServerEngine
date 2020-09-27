#include "Engine.h"

#include <iostream>

namespace SSGEServer
{

Engine::Engine()
{
	std::cout << "Server Engine Started!" << std::endl;
}

Engine::~Engine()
{
	std::cout << "Server Engine Teardown!" << std::endl;
}

Engine* getEngine()
{
	static Engine engine;
	return &engine;
}

} // namespace SSGEServer
