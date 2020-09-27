#include "Server.h"
#include "ConnectionHandler.h"

namespace SSGEServer
{

Server::Server()
{
	const unsigned int timeoutSeconds = 30;

	// Server Socket
	ServerSocket svs(2222);

	// Reactor-Notifier
	SocketReactor reactor;
	Poco::Timespan timeout(timeoutSeconds * 1000000); // 30Sec
	reactor.setTimeout(timeout);

	// Server-Acceptor
	SocketAcceptor<ConnectionHandler> acceptor(svs, reactor);

	// Threaded Reactor
	Thread thread;
	thread.start(reactor);

	// Wait for CTRL+C
	waitForTerminationRequest();

	// Stop Reactor
	reactor.stop();
	thread.join();
}

} // namespace SSGEServer
