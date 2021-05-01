#pragma once

#include <iostream>
#include "ANSIColorText.hpp"

constexpr const char* str_end(const char *str)
{
	return *str ? str_end(str + 1) : str;
}

constexpr bool str_slant(const char *str)
{
	return *str == '/' ? true : (*str ? str_slant(str + 1) : false);
}

constexpr const char* r_slant(const char* str)
{
	return *str == '/' ? (str + 1) : r_slant(str - 1);
}
constexpr const char* file_name(const char* str)
{
	return str_slant(str) ? r_slant(str_end(str)) : str;
}

#define LOG_ERR(err) \
{ \
	constexpr const char *const_file = file_name(__FILE__); \
	std::cerr << ANSI_RED "Error" ANSI_RESET ":   [" << ANSI_MAGENTA << const_file << ANSI_RESET << ':' << ANSI_CYAN << __LINE__ << ANSI_RESET << "] " << err << std::endl; \
}

#define LOG_WARN(err) \
{ \
	constexpr const char *const_file = file_name(__FILE__); \
	std::cout << ANSI_YELLOW "Warning" ANSI_RESET ": [" << ANSI_MAGENTA << const_file << ANSI_RESET << ':' << ANSI_CYAN << __LINE__ << ANSI_RESET << "] " << err << std::endl; \
}
