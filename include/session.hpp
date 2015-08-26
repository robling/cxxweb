#ifndef INCLUDE_SESSION_HPP_
#define INCLUDE_SESSION_HPP_

#include "stdafx.h"
#include <unordered_set>

namespace cxxweb
{
	using std::cout;
	using std::endl;

	using boost::asio::ip::tcp;

	class http;
	class session_manager;

	class session_base :
		public std::enable_shared_from_this<session_base>
	{
		friend session_manager;
	public:
		virtual bool is_expired() = 0;
		virtual void go() = 0;
	};

	class session : public session_base
	{
		friend session_manager;
	public:
		explicit session(std::shared_ptr<session_manager> manager, std::shared_ptr<tcp::socket> sock);
		virtual ~session();
		virtual void go();
		virtual bool is_expired() {
			return this->timer_.expires_from_now() <= std::chrono::seconds(0);
		}
	private:
		std::array<char, 8192> buffer_;
		boost::asio::steady_timer timer_;
		std::weak_ptr<session_manager> manager_;
		std::shared_ptr<tcp::socket> socket_;
		boost::asio::io_service::strand strand_;

		void release();
	};

	class session_manager :
		public std::enable_shared_from_this<session_manager>
	{
		typedef std::shared_ptr<session_base> session_ptr;
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
}

#endif //INCLUDE_SESSION_HPP_