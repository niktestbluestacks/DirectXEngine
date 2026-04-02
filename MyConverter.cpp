#include "MyConverter.hpp"

std::unique_ptr<const std::wstring> MyConverter::constCharPtrPathToWstring(const char* str) {
	const std::filesystem::path StrPath = str;
	std::string StrPathToStr = std::filesystem::absolute(StrPath).string();
	std::replace(StrPathToStr.begin(), StrPathToStr.end(), '\\', '/');
	const std::wstring WStrPathToStr = std::wstring(StrPathToStr.begin(), StrPathToStr.end());
	return std::make_unique <const std::wstring>(WStrPathToStr);
}

std::unique_ptr<const std::string> MyConverter::constCharPtrPathToString(const char* str) {
	const std::filesystem::path StrPath = str;
	std::string StrPathToStr = std::filesystem::absolute(StrPath).string();
	std::replace(StrPathToStr.begin(), StrPathToStr.end(), '\\', '/');
	return std::make_unique <const std::string>(StrPathToStr);
}
