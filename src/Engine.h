#pragma once

#include <SSGE/Engine.h>
#include "PhysicsWorld.h"
#include "CollisionEntity.h"

#include <vector>

#include <Poco/Thread.h>
#include <Poco/Runnable.h>

namespace SSGEServer
{

class Engine :
	public SSGE::Engine,
	public SSGEServer::PhysicsWorld,
	public Poco::Runnable

{
private:
	bool isStopped = false;
	Poco::Thread simulationThread;

public:
	bool shouldRun;
	int tickTimeMs = 30;
	std::vector<CollisionEntity> entities;

	Engine();
	~Engine();

	/**
	 * Start the simulation on a seperate thread
	 */
	void start();

	/**
	 * Join the simulation thread
	 */
	void stop();

	/**
	 * Thread worker/Game loop
	 */
	void run();

	/**
	 * Get epoch time in milliseconds
	 *
	 * @return Time in milliseconds
	 */
	unsigned __int64 getTime();
};

Engine* getEngine();

} // namespace SSGEServer
