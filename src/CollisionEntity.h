#pragma once

#include <SSGE/CollisionEntity.h>

namespace SSGEServer
{

/**
 * A CollisionEntity is an Entity attached to the physics engine
 */
class CollisionEntity : public SSGE::CollisionEntity
{
protected:
	friend class Database;
};

} // namespace SSGEServer
