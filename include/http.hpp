#ifndef INCLUDE_HTTP_HPP_
#define INCLUDE_HTTP_HPP_

#include "stdafx.h"

using std::cout;
using std::endl;

using boost::asio::ip::tcp;

namespace cxxweb
{
	class session_manager;
	class http
	{
	public:
		explicit http(int port);
		inline boost::asio::io_service& get_io_service() {
			return this->ioservice_;
		}
		inline tcp::acceptor& get_tcp_acceptor() {
			return this->accepter_;
		}
		void run();
		//void use();
	private:
		void listern();
		std::shared_ptr<session_manager> session_manager_;
		boost::asio::io_service ioservice_;
		tcp::acceptor accepter_;
	};
}

#endif //INCLUDE_HTTP_HPP_