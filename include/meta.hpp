#ifndef INCLUDE_META_HPP_
#define INCLUDE_META_HPP_
#include "../include/stdafx.h"

namespace cxxweb
{
	namespace http
	{
		class code
		{
		public:
			std::string operator[](int c);
		};
	}
}


#endif //INCLUDE_META_HPP_