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
	std::unique_ptr <const std::wstring> constCharPtrPathToWstring(const char* str) {
		const std::filesystem::path StrPath = str;
		std::string StrPathToStr = std::filesystem::absolute(StrPath).string();
		std::replace(StrPathToStr.begin(), StrPathToStr.end(), '\\', '/');
		const std::wstring WStrPathToStr = std::wstring(StrPathToStr.begin(), StrPathToStr.end());
		return std::make_unique <const std::wstring>(WStrPathToStr);
	}
}

#endif // _MYCONVERTER_HPP_