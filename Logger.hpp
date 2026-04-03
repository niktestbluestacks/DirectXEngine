#pragma once
#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_
// Includes
#include <format>
#include <stdexcept>
#include <string>

// My includes
#include "ClassMacroes.hpp"

namespace dxed {
	class Logger final {
	dxed_disable_copy_and_move(Logger)
	public:
		enum class LogLevel {
			Error = 0,
			Warning,
			Info
		};

		explicit Logger(LogLevel logLevel = LogLevel::Error);
		Logger();
		~Logger();

		template <typename... Args>
		void log(LogLevel, std::format_string <Args...>&, Args&&...);

		void logInfo(const char*);
	private:
		void _log(LogLevel logLevel, const char* message);
	private:
		LogLevel m_logLevel = LogLevel::Error;
	};
}

#endif // _LOGGER_HPP_

