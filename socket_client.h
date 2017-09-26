#ifdef TEMPORARY_EXCLUDED

#pragma once

#include <boost/asio.hpp>

#include "reliable_serial.h"

typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_sp;

class socket_client : public reliable_serial
{
	boost::asio::ip::tcp::socket socket;

	std::string					host;
	int							port;

	bool						is_connected;

	char						buffer[256];

	void						connect						();
	void						connect_handler				(const boost::system::error_code &error);
	void						data_ready					(const boost::system::error_code &e, size_t bytes_read);

public:
	/*constructor*/				socket_client				(std::string host, int port);
	/*destructor*/				~socket_client				();

	size_t						write						(const void *data, size_t size);
};

#endif
