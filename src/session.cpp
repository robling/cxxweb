#include "../include/http.hpp"
#include "../include/session.hpp"

using boost::asio::spawn;
using boost::asio::yield_context;


const std::string http200_ = "HTTP/1.1 200 OK\r\n";
const std::string body_ = R"(
<!DOCTYPE html>
<head></head>
<body>
	<h1>Hello World!</h1>
</body>
)";

cxxweb::session::session(std::shared_ptr<session_manager> manager, std::shared_ptr<tcp::socket> sock) :
	manager_(manager),
	socket_(sock),
	strand_(sock->get_io_service()),
	timer_(sock->get_io_service())
{
}

cxxweb::session::~session()
{
	this->socket_->close();
}

void cxxweb::session::go()
{
	auto self = shared_from_this();
	spawn(strand_, [this, self](yield_context yield) {
		boost::system::error_code ec;
		boost::asio::async_write(*(this->socket_), boost::asio::buffer(http200_ + body_), yield[ec]);
		this->socket_->shutdown(tcp::socket::shutdown_send);
		this->timer_.expires_from_now(std::chrono::seconds(10));
	});

	//Since asio use stackful coroutine, this method requires toooo much memery
	/*spawn(strand_, [this, self](yield_context yield) {
	while (self.use_count() != 1)
	{
	boost::system::error_code ec;
	this->timer_.async_wait(yield[ec]);
	if (timer_.expires_from_now() <= std::chrono::seconds(0))
	{
	this->manager_.lock()->stop(shared_from_this());
	}
	}
	});*/
}

cxxweb::session_manager::session_manager(http& server) : server_(server), strand_(server.get_io_service())
{
	this->gc_loop();
}

cxxweb::session_manager::~session_manager()
{
	this->stop_all();
}

cxxweb::session_manager::session_ptr cxxweb::session_manager::start(boost::asio::yield_context& yield)
{
	try
	{
		boost::system::error_code ec;

		auto socket_ = std::make_shared<tcp::socket>(this->strand_.get_io_service());
		auto session_ = std::make_shared<session>(shared_from_this(), socket_);
		this->session_pool.insert(session_);
		this->server_.get_tcp_acceptor().async_accept(*socket_, yield[ec]);
		if (!ec)
			return session_;
		else if (ec != boost::asio::error::operation_aborted)
		{
			this->stop(session_);
			throw (std::exception(__FILE__));
		}
	}
	catch (std::exception e)
	{
		//TODO::log
		throw(std::exception(e.what()));
	}
	catch (...)
	{
		std::cerr << "UNEXPECTED ERROR HUPPEND" << std::endl;
		throw (std::exception(__FILE__));
	}
}

void cxxweb::session_manager::stop(cxxweb::session_manager::session_ptr s)
{
	this->session_pool.erase(s);
}

void cxxweb::session_manager::stop_all()
{
	for (auto& s_p : this->session_pool) {
		this->session_pool.erase(s_p);
	}
	session_pool.clear();
}

size_t cxxweb::session_manager::get_count()
{
	return this->session_pool.size();
}

void cxxweb::session_manager::gc_loop()
{
	spawn(this->strand_, [this](yield_context yield) {
		boost::asio::steady_timer timer_(this->strand_.get_io_service());
		//timer_.expires_from_now(std::chrono::seconds(3));
		for (;;)
		{
			boost::system::error_code ec;
			timer_.async_wait(yield);
			for (auto s_p = this->session_pool.begin(); s_p != session_pool.end(); )
			{
				timer_.async_wait(yield);
				if ((*s_p)->timer_.expires_from_now() <= std::chrono::seconds(0))
				{
					s_p = this->session_pool.erase(s_p);
					continue;
				}
				s_p++;
			}
			timer_.expires_from_now(std::chrono::seconds(5));
		}
	});
}