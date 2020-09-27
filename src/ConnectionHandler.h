#pragma once

#include <Poco/Net/SocketReactor.h>
#include <Poco/Net/SocketAcceptor.h>
#include <Poco/Net/SocketNotification.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/NObserver.h>
#include <Poco/Exception.h>
#include <Poco/Thread.h>
#include <Poco/FIFOBuffer.h>
#include <Poco/Delegate.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>

#include <iostream>
#include <vector>
#include <string>

using Poco::Net::SocketReactor;
using Poco::Net::SocketAcceptor;
using Poco::Net::ReadableNotification;
using Poco::Net::WritableNotification;
using Poco::Net::ShutdownNotification;
using Poco::Net::ErrorNotification;
using Poco::Net::TimeoutNotification;
using Poco::Net::ServerSocket;
using Poco::Net::StreamSocket;
using Poco::NObserver;
using Poco::AutoPtr;
using Poco::Thread;
using Poco::FIFOBuffer;
using Poco::delegate;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;

#ifndef CONN_BUFFER_SIZE
#define CONN_BUFFER_SIZE 1024
#endif

namespace SSGEServer
{

class ConnectionHandler
{
protected:
	/// Stream Socket
	StreamSocket socket;

	/// Socket Reactor-Notifier
	SocketReactor& reactor;

	FIFOBuffer _fifoIn;
	FIFOBuffer _fifoOut;

	/**
	 * Read bytes from the socket, depending on available bytes on socket
	 */
	void readBytes();

	/**
	 * Send message to the socket
	 * @param std::string is the message (null terminated)
	 */
	void sendMessage(std::string);

	void onFIFOOutReadable(bool& b);
	void onFIFOInWritable(bool& b);

public:
	/**
	 * Constructor of the Connection Handler
	 *
	 * @note Each object is unique to an accepted connection
	 * @see SocketAcceptor http://pocoproject.org/docs/Poco.Net.SocketAcceptor.html
	 *
	 * @param SteamSocket& _socket The socket accepting the connections
	 * @param SocketReactor& _reactor The reacting engine (threaded) which creates notifications about the socket
	 */
	ConnectionHandler(StreamSocket& _socket, SocketReactor& _reactor);

	/**
	 * Destructor
	 */
	~ConnectionHandler();

	/**
	* Event Handler when Socket becomes Readable, i.e: there is data waiting to be read
	*/
	void onSocketReadable(const AutoPtr<ReadableNotification>& pNf);

	/**
	* Event Handler when Socket was written, i.e: confirmation of data sent away (not received by client)
	*/
	void onSocketWritable(const AutoPtr<WritableNotification>& pNf);

	/**
	* Event Handler when Socket was shutdown on the remote/peer side
	*/
	void onSocketShutdown(const AutoPtr<ShutdownNotification>& pNf);

	/**
	* Event Handler when Socket throws an error
	*/
	void onSocketError(const AutoPtr<ErrorNotification>& pNf);

	/**
	* Event Handler when Socket times-out
	*/
	void onSocketTimeout(const AutoPtr<TimeoutNotification>& pNf);
};

} // namespace SSGEServer
