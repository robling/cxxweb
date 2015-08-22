#include "stdafx.h"

using std::cout;
using std::endl;

using boost::asio::ip::tcp;

namespace cxxweb
{
	class http
	{
	public:
		explicit http(int port);
		void run();
		void use();
	private:
	};
}