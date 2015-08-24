#include "stdafx.h"
#include <unordered_set>

using std::cout;
using std::endl;

using boost::asio::ip::tcp;

namespace cxxweb
{
	class http;
	class session_manager;

	class session : 
		public std::enable_shared_from_this<session>
	{
		friend session_manager;
	public:
		explicit session(std::shared_ptr<session_manager> manager, std::shared_ptr<tcp::socket> sock);
		~session();
		void go();
	private:
		boost::asio::steady_timer timer_;
		std::weak_ptr<session_manager> manager_;
		std::shared_ptr<tcp::socket> socket_;
		boost::asio::io_service::strand strand_;
	};

	class session_manager :
		public std::enable_shared_from_this<session_manager>
	{
		typedef std::shared_ptr<session> session_ptr;
	public:
		session_manager(http& server);
		~session_manager();
		session_ptr start(boost::asio::yield_context& yield);
		void stop(session_ptr s);
		void stop_all();
		size_t get_count();
		void gc_loop();
	private:
		http& server_;
		boost::asio::io_service::strand strand_;
		std::unordered_set<session_ptr> session_pool;
	};

	class http
	{
	public:
		explicit http(int port);
		inline boost::asio::io_service& get_io_service();
		inline tcp::acceptor& get_tcp_acceptor();
		void run();
		//void use();
	private:
		void listern();
		std::shared_ptr<session_manager> session_manager_;
		boost::asio::io_service ioservice_;
		tcp::acceptor accepter_;
	};
}