#pragma once

#include <SSGE/Engine.h>

namespace SSGEServer
{

class Engine :
	public SSGE::Engine
{
public:
	Engine();
	~Engine();
};

Engine* getEngine();

} // namespace SSGEServer
