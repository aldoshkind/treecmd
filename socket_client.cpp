#include "socket_client.h"

#include "boost/bind.hpp"

#include "io_service.h"

/*constructor*/ socket_client::socket_client(std::string host, int port) : socket(treeipc_over_boost::get_io_service())
{
	this->host = host;
	this->port = port;

	is_connected = false;

	connect();
}

/*destructor*/ socket_client::~socket_client()
{
	//
}

void socket_client::connect_handler(const boost::system::error_code &error)
{
	if(error)
	{
		printf("%s %s %d\n", __PRETTY_FUNCTION__, error.message().c_str(), socket.is_open());
		is_connected = false;
		return;
	}

	is_connected = true;

	socket.async_receive(boost::asio::buffer(buffer, sizeof(buffer)),
						 0,
						 boost::bind(&socket_client::data_ready,
									 this,
									 boost::asio::placeholders::error,
									 boost::asio::placeholders::bytes_transferred
									 )
						 );
}

size_t socket_client::write(const void *data, size_t size)
{
	if(is_connected == false)
	{
		return 0;
	}

	boost::system::error_code ec;
	size_t written = socket.write_some(boost::asio::buffer(data, size), ec);

	if(ec.value() != 0)
	{
		printf("%s %s %d\n", __PRETTY_FUNCTION__, ec.message().c_str(), socket.is_open());
		is_connected = false;
		return 0;
	}

	return written;
}

void socket_client::connect()
{
	socket.close();
	socket.open(boost::asio::ip::tcp::v4());
	socket.async_connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(host), port),
						 boost::bind(&socket_client::connect_handler, this, boost::asio::placeholders::error)
						 );
}

void socket_client::data_ready(const boost::system::error_code &e, size_t bytes_read)
{
	if(e.value() != 0)
	{
		printf("%s %s %d\n", __PRETTY_FUNCTION__, e.message().c_str(), socket.is_open());
		is_connected = false;
		return;
	}


	notify(buffer, bytes_read);

	socket.async_receive(boost::asio::buffer(buffer, sizeof(buffer)),
						 0,
						 boost::bind(&socket_client::data_ready,
									 this,
									 boost::asio::placeholders::error,
									 boost::asio::placeholders::bytes_transferred
									 )
						 );
}
