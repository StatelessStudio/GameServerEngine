#include "ConnectionHandler.h"
#include "Engine.h"
#include <SSGE/Entity.h>

bool hasRunSimulation = false;

namespace SSGEServer
{

ConnectionHandler::ConnectionHandler(StreamSocket& _socket, SocketReactor& _reactor) :
	socket(_socket),
	reactor(_reactor),
	_fifoIn(CONN_BUFFER_SIZE, true),
	_fifoOut(CONN_BUFFER_SIZE, true)
{
	std::cout << "[" << socket.peerAddress().toString() << "] Joined the server" << std::endl;

	reactor.addEventHandler(socket, NObserver<ConnectionHandler, ReadableNotification>(*this, &ConnectionHandler::onSocketReadable));
	reactor.addEventHandler(socket, NObserver<ConnectionHandler, ShutdownNotification>(*this, &ConnectionHandler::onSocketShutdown));
	reactor.addEventHandler(socket, NObserver<ConnectionHandler, ErrorNotification>(*this, &ConnectionHandler::onSocketError));
	reactor.addEventHandler(socket, NObserver<ConnectionHandler, TimeoutNotification>(*this, &ConnectionHandler::onSocketTimeout));

	_fifoOut.readable += delegate(this, &ConnectionHandler::onFIFOOutReadable);
	_fifoIn.writable += delegate(this, &ConnectionHandler::onFIFOInWritable);
	
	if (!hasRunSimulation) {
		hasRunSimulation = true;

		Engine* engine = getEngine();
		engine->shouldRun = true;
	}
}

ConnectionHandler::~ConnectionHandler()
{
	std::cout << "[" << socket.peerAddress().toString() << "] Left the server" << std::endl;

	reactor.removeEventHandler(socket, NObserver<ConnectionHandler, ReadableNotification>(*this, &ConnectionHandler::onSocketReadable));
	reactor.removeEventHandler(socket, NObserver<ConnectionHandler, WritableNotification>(*this, &ConnectionHandler::onSocketWritable));
	
	reactor.removeEventHandler(socket, NObserver<ConnectionHandler, ShutdownNotification>(*this, &ConnectionHandler::onSocketShutdown));
	reactor.removeEventHandler(socket, NObserver<ConnectionHandler, ErrorNotification>(*this, &ConnectionHandler::onSocketError));
	reactor.removeEventHandler(socket, NObserver<ConnectionHandler, TimeoutNotification>(*this, &ConnectionHandler::onSocketTimeout));

	_fifoOut.readable -= delegate(this, &ConnectionHandler::onFIFOOutReadable);
	_fifoIn.writable -= delegate(this, &ConnectionHandler::onFIFOInWritable);
}

void ConnectionHandler::readBytes()
{
	// TODO:
}

void ConnectionHandler::sendMessage(std::string)
{
	// TODO:
}

void ConnectionHandler::onFIFOOutReadable(bool& b)
{
	std::cout << "[" << socket.peerAddress().toString() << "] Reading..." << std::endl;

	if (b)
		reactor.addEventHandler(socket, NObserver<ConnectionHandler, WritableNotification>(*this, &ConnectionHandler::onSocketWritable));
	else
		reactor.removeEventHandler(socket, NObserver<ConnectionHandler, WritableNotification>(*this, &ConnectionHandler::onSocketWritable));
}

void ConnectionHandler::onFIFOInWritable(bool& b)
{
	std::cout << "[" << socket.peerAddress().toString() << "] Sending..." << std::endl;

	if (b)
		reactor.addEventHandler(socket, NObserver<ConnectionHandler, ReadableNotification>(*this, &ConnectionHandler::onSocketReadable));
	else
		reactor.removeEventHandler(socket, NObserver<ConnectionHandler, ReadableNotification>(*this, &ConnectionHandler::onSocketReadable));
}

void ConnectionHandler::onSocketReadable(const AutoPtr<ReadableNotification>& pNf)
{
	static Engine* engine = getEngine();

	std::cout << "[" << socket.peerAddress().toString() << "] Socket Readable" << std::endl;

	try {
		int len = socket.receiveBytes(_fifoIn);

		if (len <= 0) {
			std::cout << "[" << socket.peerAddress().toString() << "] Empty - Removing!" << std::endl;
			delete this;
			return;
		}

		char buffer[1024];
		_fifoIn.peek(buffer, 1024);

		int nEntities = engine->entities.size();
		for (int i = 0; i < nEntities; i++) {
			std::string data = engine->entities.at(i).serialize();
			_fifoOut.write(data.c_str(), data.size());

			SSGE::Entity entity;
			entity.unserialize(data);
		}
	}
	catch (Poco::Exception& ex) {
		std::cout << "[" << socket.peerAddress().toString() << "] " << ex.what() << std::endl;
		delete this;
	}
}

void ConnectionHandler::onSocketWritable(const AutoPtr<WritableNotification>& pNf)
{
	std::cout << "[" << socket.peerAddress().toString() << "] Socket Writeable" << std::endl;
	
	try {
		socket.sendBytes(_fifoOut);
	}
	catch (Poco::Exception& ex) {
		std::cout << "[" << socket.peerAddress().toString() << "] " << ex.what() << std::endl;
		delete this;
	}
}

void ConnectionHandler::onSocketShutdown(const AutoPtr<ShutdownNotification>& pNf)
{
	std::cout << "[" << socket.peerAddress().toString() << "] Socket Shutdown" << std::endl;
	delete this;
}

void ConnectionHandler::onSocketError(const AutoPtr<ErrorNotification>& pNf)
{
	std::cout << "[" << socket.peerAddress().toString() << "] Socket Error" << std::endl;
	delete this;
	// TODO:
}

void ConnectionHandler::onSocketTimeout(const AutoPtr<TimeoutNotification>& pNf)
{
	std::cout << "[" << socket.peerAddress().toString() << "] Socket Timeout" << std::endl;
	//delete this;
	// TODO:
}

} // namespace SSGEServer
