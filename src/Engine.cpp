#include "Engine.h"

#include <chrono>
#include <iostream>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;

namespace SSGEServer
{

void Engine::start()
{
	thread.start(this);
}

void Engine::stop()
{
	isStopped = true;
	thread.join();
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

Engine* getEngine()
{
	static Engine engine;
	return &engine;
}

} // namespace SSGEServer
