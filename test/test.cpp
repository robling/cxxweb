#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "../include/stdafx.h"
#include <future>
#include <thread>
#include <chrono>
#include <boost/any.hpp>

BOOST_AUTO_TEST_CASE(regexp)
{
	std::string str = "Hello World";
	static const boost::regex exp{ R"(^H.+ld$)" };
	BOOST_CHECK(boost::regex_match(str, exp));
}
BOOST_AUTO_TEST_CASE(coroutine)
{
	int i = 100;
	int rst = 0;
	using boost::coroutines::symmetric_coroutine;
	symmetric_coroutine<void>::call_type *p_coro_a = 0, *p_coro_b = 0;
	symmetric_coroutine<void>::call_type coro_a(
		[&](boost::coroutines::symmetric_coroutine<void>::yield_type& yield) {
		if (i != 0){
			i--;
			yield(*p_coro_b);
		}
		});
	symmetric_coroutine<void>::call_type coro_b(
		[&](boost::coroutines::symmetric_coroutine<void>::yield_type& yield) {
		if (i != 0) {
			i--;
			yield(*p_coro_a);
		}
	});
	p_coro_a = &coro_a;
	p_coro_b = &coro_b;

	coro_a(); // enter
	BOOST_CHECK(rst == 0);
}
BOOST_AUTO_TEST_CASE(std_theard)
{
	using std::future;
	using std::thread;
	auto start = std::chrono::high_resolution_clock::now();
	future<int> grst = std::async(std::launch::async, []() {std::this_thread::sleep_for(std::chrono::seconds(5));return 100;});
	auto rst = grst.get();
	auto end = std::chrono::high_resolution_clock::now();
	//useful helper under namespace chrono
	//but function name is tooooo long
	std::cout << "Waited:" << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "ms" << std::endl;
	BOOST_CHECK(rst == 100);
}
BOOST_AUTO_TEST_CASE(boost_any)
{
	boost::any str;
	str = std::string(R"(ABCD)");
	bool A = false;
	int i = 0;
	try {
		i = boost::any_cast<int>(str);
	}
	catch (...) {
		A = true;
	}
	auto rst_str = boost::any_cast<std::string>(str);
	BOOST_CHECK(A);
	BOOST_CHECK(0 == rst_str.compare("ABCD"));
}