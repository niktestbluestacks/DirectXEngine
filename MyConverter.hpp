#pragma once
#ifndef _MYCONVERTER_HPP_
#define _MYCONVERTER_HPP_

// Includes

#include <memory>
#include <string>
#include <algorithm>
#include <filesystem>

// namespace MyConverter
namespace MyConverter {
	std::unique_ptr <const std::wstring> constCharPtrPathToWstring(const char* str);

	std::unique_ptr <const std::string> constCharPtrPathToString(const char* str);
}

#endif // _MYCONVERTER_HPP_