#include "..\include\http.hpp"

cxxweb::http::http(int port) :
	ioservice_(),
	accepter_(ioservice_, tcp::endpoint(tcp::v4(), port))
{

}

void cxxweb::http::run()
{
	ioservice_.run();
}

cxxweb::session::session(boost::asio::io_service & ioservice)
{
}
