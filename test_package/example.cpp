#include <iostream>

#include <SSGE/Exception.h>

#include <SSGEServer/Engine.h>
#include <SSGEServer/Database.h>
#include <SSGEServer/CollisionEntity.h>
#include <SSGEServer/Server.h>

using std::vector;
using std::cout;
using std::endl;
using SSGE::Vec3;
using namespace SSGEServer;

int main()
{
    try {
        cout << "Creating engine..." << endl;
        Engine* engine;
        engine = getEngine();
        engine->shouldRun = false;

        std::vector<CollisionEntity>* entities = &engine->entities;

        cout << "Setting up database..." << endl;
        Database db("SQLite", "C:/Users/Drew/Downloads/sample.db");

        cout << "Migrating... " << endl;
        db.migrate();

        cout << "Seeding... " << endl;
        db.seed();


        cout << "Getting entities..." << endl;
        db.getCollisionEntities(*entities);

        cout << "Found " << entities->size() << " entities!" << endl;

        // Output found entities
        for (int i = 0; i < entities->size(); i++) {
            cout << "ENTITY " << i << " address: " << &(entities->at(i)) << endl;

            Vec3 pos = entities->at(i).getPosition();
            cout << pos.x << " " << pos.y << " " << pos.z << endl;

            engine->addEntity(entities->at(i));
        }

        // Run simulation
        engine->start();

        // Start server
        SSGEServer::Server server;

        // Kill engine when server terminates
        engine->stop();
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
