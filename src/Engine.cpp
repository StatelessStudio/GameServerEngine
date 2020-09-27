#include "Engine.h"

#include <chrono>
#include <iostream>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;

namespace SSGEServer
{

Engine::Engine()
{
	std::cout << "Server Engine Started!" << std::endl;

	shouldRun = true;
}

Engine::~Engine()
{
	std::cout << "Server Engine Teardown!" << std::endl;
}

void Engine::start()
{
	simulationThread.start(this);
}

void Engine::stop()
{
	isStopped = true;
	simulationThread.join();
}

void Engine::run()
{
	std::cout << "Starting engine!" <<std::endl;
	unsigned __int64 lastrun = this->getTime();

	while (true) {
		if (isStopped) {
			break;
		}

		unsigned __int64 now = this->getTime();
		if (shouldRun && now >= (lastrun + tickTimeMs)) {
			this->step();
			lastrun = now;
		}
	}
}

unsigned __int64 Engine::getTime()
{
	return duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
	).count();
}

Engine* getEngine()
{
	static Engine engine;
	return &engine;
}

} // namespace SSGEServer
