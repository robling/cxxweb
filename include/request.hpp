#ifndef INCLUDE_REQUEST_HPP_
#define INCLUDE_REQUEST_HPP_

#include <list>
#include <string>

#include <boost/logic/tribool.hpp>

namespace cxxweb
{
	class Request
	{
	public:
		inline void append(std::string&& str)
		{
			this->req_str_.push_back(str);
		}
		inline void append(std::string& str)
		{
			this->append(std::move(str));
		}
		std::list<std::string> req_str_;
		inline boost::tribool get_status()
		{
			return this->status_;
		}
	private:
		boost::tribool status_;
	};
}

#endif //INCLUDE_REQUEST_HPP_