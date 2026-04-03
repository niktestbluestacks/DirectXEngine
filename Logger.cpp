#include "Logger.hpp"
#include <iostream>

using namespace dxed;

Logger::Logger(LogLevel logLevel) : m_logLevel(logLevel) {}

dxed::Logger::Logger() = default;

Logger::~Logger() {}

template <typename... Args>
void Logger::log(LogLevel level, std::format_string <Args ...>& fmt, Args&& ... args) {
	auto str = std::format(fmt, std::forward <Args>(args)...);
	_log(level,
		str.c_str()
	);
}

void Logger::logInfo(const char* info) {
	this->_log(LogLevel::Info, info);
}

void Logger::_log(LogLevel level, const char* message) {
	auto logLevelToString = [](LogLevel level) {
		switch(level) {
		case LogLevel::Info: return "Info";
		case LogLevel::Warning: return "Warning";
		case LogLevel::Error: return "Error";
		default: return "Unknown";
		}
	};

	if (level > m_logLevel) { return; }
	std::clog << "[DXED " << logLevelToString(level) << "]:" << message << "\n";
}