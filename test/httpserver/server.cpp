#include "../../include/stdafx.h"
#include "../../include/http.hpp"

using std::string;

int main()
{
	std::cout << "Hello World!" << std::endl;
	cxxweb::http app(8080);
	app.run();
#ifdef _MSC_VER
	system("pause");
#endif
}