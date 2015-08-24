#include "stdafx.h"
#include <unordered_set>

using std::cout;
using std::endl;

using boost::asio::ip::tcp;

namespace cxxweb
{
	class session
	{
	public:
		explicit session(boost::asio::io_service& ioservice);
	};

	class session_manager
	{
		typedef std::shared_ptr<session> session_ptr;
	public:
		session_ptr start();
		void stop(session_ptr s);
		void stop_all();
		size_t get_count();
	private:
		std::unordered_set<session_ptr> session_pool;
	};

	class http
	{
	public:
		explicit http(int port);
		void run();
		void use();
	private:
		boost::asio::io_service ioservice_;
		tcp::acceptor accepter_;
	};
}