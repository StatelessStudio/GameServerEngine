#include <vector>
#include <string>

#include <Poco/Data/SQLite/Connector.h>

#include <SSGE/Vec3.h>
#include <SSGE/Exception.h>

#include "Database.h"

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

using SSGE::Vec3;

using std::string;

#define randfloat(A) static_cast<float> ((rand()) / static_cast <float> (RAND_MAX)) * A

namespace SSGEServer
{

Database::Database(const char* _type, const char* _conn)
{
	type = _type;
	conn = _conn;

	Poco::Data::SQLite::Connector::registerConnector();
	session = new Session(type, conn);
}

Database::~Database()
{
	delete session;
}

void Database::migrate()
{
	try {
		*session << "DROP TABLE IF EXISTS CollisionEntity", now;
	}
	catch (Poco::Exception& ex) {
		string message("Could not drop table CollisionEntity: ");
		message.append(ex.what());

		throw SSGE::Exception(message.c_str());
	}

	try {
		*session << "CREATE TABLE CollisionEntity (" <<
			"id INTEGER PRIMARY KEY, " <<
			"posx REAL, " <<
			"posy REAL, " <<
			"posz REAL, " <<
			"scalex REAL, " <<
			"scaley REAL, " <<
			"scalez REAL, " <<
			"mass REAL" <<
		")", now;
	}
	catch (Poco::Exception& ex) {
		string message("Could not create table CollisionEntity: ");
		message.append(ex.what());

		throw SSGE::Exception(message.c_str());
	}
}

void Database::seed(unsigned int nEntities)
{
	CollisionEntity entity;

	// Floor
	entity.setPosition(Vec3(0, -2.5f, 0));
	entity.setScale(Vec3(100, 5, 100));
	entity.setMass(0);

	try {
		Statement insert(*session);
		insert << "INSERT INTO CollisionEntity VALUES(NULL, ?, ?, ?, ?, ?, ?, ?)",
			use(entity.position.x),
			use(entity.position.y),
			use(entity.position.z),
			use(entity.scale.x),
			use(entity.scale.y),
			use(entity.scale.z),
			use(entity.mass);

		insert.execute();

		for (int i = 0; i < nEntities; i++) {
			entity.setPosition(Vec3(
				randfloat(50),
				randfloat(50),
				randfloat(50)
			));
			entity.setScale(Vec3(1, 1, 1));
			entity.setMass(randfloat(1));

			insert.execute();
		}
	}
	catch (Poco::Exception& ex) {
		string message("Could not insert CollisionEntity: ");
		message.append(ex.what());

		throw SSGE::Exception(message.c_str());
	}
}

void Database::getCollisionEntities(std::vector<CollisionEntity>& entities)
{
	try {
		CollisionEntity entity;
		Statement select(*session);
		select << "SELECT * FROM CollisionEntity",
			into(entity.id),
			into(entity.position.x),
			into(entity.position.y),
			into(entity.position.z),
			into(entity.scale.x),
			into(entity.scale.y),
			into(entity.scale.z),
			into(entity.mass),
			range(0, 1); //  Iterate over result set one row at a time

		while (!select.done())
		{
			select.execute();
			entity.hasId = true;
			entities.push_back(entity);
		}
	}
	catch (Poco::Exception& ex) {
		string message("Could not query table CollisionEntity: ");
		message.append(ex.what());

		throw SSGE::Exception(message.c_str());
	}
}

} // namespace SSGEServer
