#pragma once

#include <Poco/Data/Session.h>

#include "PhysicsWorld.h"
#include "CollisionEntity.h"

#include <vector>

namespace SSGEServer
{

/**
 * Load game state from a database
 */
class Database
{
protected:
	const char* type;
	const char* conn;

	Poco::Data::Session* session = nullptr;

public:
	/**
	 * Create a database connection
	 * 
	 * @param {const char*} _type Connection type "MySQL", "SQLite", etc
	 * @param {const char*} _conn Connection string or SQLite database file
	 */
	Database(const char* _type, const char* _conn);

	/**
	 * Close the session
	 */
	~Database();

	/**
	 * Migrate the database tables
	 */
	void migrate();

	/**
	 * Seed the database with sample data
	 * 
	 * @param {unsigned int} nEntities (Optional) Number of entities to seed
	 * 	the entity table with (plus the floor). Default is 10. Set to 0 to
	 * 	only insert the floor.
	 */
	void seed(unsigned int nEntities = 10);

	/**
	 * Get collision entities
	 * 
	 * @param {std::vector<CollisionEntity>&} entities Vector to push entities to
	 */
	void getCollisionEntities(std::vector<CollisionEntity>& entities);
};

} // namespace SSGEServer
