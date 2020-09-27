#pragma once

#include <SSGE/Engine.h>
#include "PhysicsWorld.h"
#include "CollisionEntity.h"

#include <vector>
#include <Poco/Runnable.h>

namespace SSGEServer
{

class Engine :
	public SSGE::Engine,
	public SSGEServer::PhysicsWorld,
	public Poco::Runnable
{

public:
	std::vector<CollisionEntity> entities;

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
};

Engine* getEngine();

} // namespace SSGEServer
