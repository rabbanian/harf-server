#pragma once
#include <vector>
#include <tuple>
#include <string_view>
#include <string>

class db
{
	private:
		std::vector<std::tuple<std::string, int>> users;
		db() = default;
		~db() = default;

	public:
		db(const db& src) = delete;
		db(const db&& src) = delete;
		db & operator = (const db & rhs) = delete;
		db & operator = (const db && rhs) = delete;

		static db & instance();
		void insert(std::string_view name, int fd);
		void remove_name(std::string_view name);
		void remove_fd(int fd);
		bool is_registered(std::string_view name);
		unsigned int count();
		int get_fd(std::string_view name);
		std::string_view get_name(int fd);

};
