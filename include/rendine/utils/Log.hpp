#pragma once

#include <iostream>
#include <chrono>
#include <iomanip>
#include "ANSIColorText.hpp"
#include "../Info.hpp"

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
	constexpr const char *__const_file = file_name(__FILE__); \
	time_t __time_now = time(nullptr); \
	std::cerr	<< std::put_time(localtime(&__time_now), "%d/%m/%y %T ") \
				<< std::right << std::setw(16) << __const_file  << ':' << std::left << std::setw(4) << __LINE__ \
				<< ANSI_RED << std::left << std::setw(5) << "ERROR" << ANSI_RESET << " " \
				<< err << std::endl; \
}

#define LOG_WARN(err) \
{ \
	constexpr const char *__const_file = file_name(__FILE__); \
	time_t __time_now = time(nullptr); \
	std::cout	<< std::put_time(localtime(&__time_now), "%d/%m/%y %T ") \
				<< std::right << std::setw(16) << __const_file  << ':' << std::left << std::setw(4) << __LINE__ \
				<< ANSI_YELLOW << std::left << std::setw(5) << "WARN" << ANSI_RESET << " " \
				<< err << std::endl; \
}

#define LOG_INFO(err) \
{ \
	constexpr const char *__const_file = file_name(__FILE__); \
	time_t __time_now = time(nullptr); \
	std::cout	<< std::put_time(localtime(&__time_now), "%d/%m/%y %T ") \
				<< std::right << std::setw(16) << __const_file  << ':' << std::left << std::setw(4) << __LINE__ \
				<< ANSI_CYAN << std::left << std::setw(5) << "INFO" << ANSI_RESET << " " \
				<< err << std::endl; \
}

#define LOG_EXIT(err) \
{ \
	constexpr const char *__const_file = file_name(__FILE__); \
	time_t __time_now = time(nullptr); \
	std::cerr	<< std::put_time(localtime(&__time_now), "%d/%m/%y %T ") \
				<< std::right << std::setw(16) << __const_file  << ':' << std::left << std::setw(4) << __LINE__ \
				<< ANSI_BOLDRED << std::left << std::setw(5) << "EXIT" << ANSI_RESET << " " \
				<< err << std::endl; \
}
