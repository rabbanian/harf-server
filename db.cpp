#include "db.h"
#include <algorithm>

db & db::instance()
{
	/* C++11 mandates that the initializer for a local static variable is only run once, even in
	* the presence of concurrency. So this code is thread safe */
	static db * _instance = new db();
	return *_instance;
}

void db::insert(std::string_view name, int fd)
{
	users.push_back({std::string(name), fd});
}

void db::remove_name(std::string_view name)
{
	std::remove_if(std::begin(users), std::end(users), [name](auto & u) { return std::get<0>(u) == name; });
}

void db::remove_fd(int fd)
{
	std::remove_if(std::begin(users), std::end(users), [fd](auto & u) { return std::get<1>(u) == fd; });
}

bool db::is_registered(std::string_view name)
{
	return !(std::find_if(std::begin(users), std::end(users), [name](auto & u) { return std::get<0>(u) == name; }) == std::end(users));
}

unsigned int db::count()
{
	return users.size();
}

int db::get_fd(std::string_view name)
{
	auto temp = std::find_if(std::begin(users), std::end(users), [name](auto & u) { return std::get<0>(u) == name; });
	return std::get<1>(*temp);
}

std::string_view db::get_name(int fd)
{
	auto temp = std::find_if(std::begin(users), std::end(users), [fd](auto & u) { return std::get<1>(u) == fd; });
	return std::get<0>(*temp);
}
