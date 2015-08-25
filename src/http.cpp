#include "..\include\http.hpp"
#include "..\include\session.hpp"

using boost::asio::spawn;
using boost::asio::yield_context;

cxxweb::http::http(int port) :
	ioservice_(),
	accepter_(ioservice_, tcp::endpoint(tcp::v4(), port))
{
	session_manager_ = std::make_shared<session_manager>(*this);
}

void cxxweb::http::run()
{
	this->listern();
	ioservice_.run();
	
}

void cxxweb::http::listern()
{
	spawn(this->ioservice_, [this](yield_context yield) {
		for (;;)
		{
			try
			{
				this->session_manager_->start(yield)->go();
				auto i = session_manager_->get_count();
				//if (i % 666 == 0) std::cout << i << endl;
			}
			catch (std::exception e)
			{
				std::cerr << e.what() << std::endl;
				std::terminate();
			}
		}
	});
}
