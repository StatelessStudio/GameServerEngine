#include <iostream>

#include <SSGE/Exception.h>

#include <SSGEServer/Engine.h>
#include <SSGEServer/Database.h>
#include <SSGEServer/CollisionEntity.h>
#include <SSGEServer/PhysicsWorld.h>

using std::vector;
using std::cout;
using std::endl;
using SSGE::Vec3;
using namespace SSGEServer;

int main()
{
    try {
        cout << "Creating engine..." << endl;
        Engine engine;

        cout << "Setting up database..." << endl;
        Database db("SQLite", "C:/Users/Drew/Downloads/sample.db");

        cout << "Migrating... " << endl;
        db.migrate();

        cout << "Seeding... " << endl;
        db.seed();

        cout << "Creating physics world..." << endl;
        PhysicsWorld physicsWorld;

        cout << "Getting entities..." << endl;
        vector<CollisionEntity> entities;
        db.getCollisionEntities(entities);

        cout << "Found " << entities.size() << " entities!" << endl;

        // Output found entities
        for (int i = 0; i < entities.size(); i++) {
            cout << "ENTITY " << i << " address: " << &(entities.at(i)) << endl;

            Vec3 pos = entities.at(i).getPosition();
            cout << pos.x << " " << pos.y << " " << pos.z << endl;

            physicsWorld.addEntity(entities.at(i));
        }

        // Run simulation
        for (int i = 0; i < 500; i++) {
            physicsWorld.step();

            for (int i = 0; i < entities.size(); i++) {
                CollisionEntity* entity = &entities.at(i);

                if (entity->getMass() > 0) {
                // cout << "ENTITY " << i << " address: " << entity << endl;

                    Vec3 pos = entity->getPosition();
                    cout << "Position: (" << pos.x << ", " << pos.y << ", " << pos.z <<
                        ")" << endl;
                }
            }
        }
    }
    catch (SSGE::Exception& ex) {
        cout << "[SSGE] " << ex.what() << endl;
        
        return 1;
    }
    catch (std::exception& ex) {
        cout << "[ERROR] " << ex.what() << endl;

        return 2;
    }

    return 0;
}
