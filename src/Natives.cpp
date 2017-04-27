#include "Config.hpp"
#include <cstdlib>
#include "Logprintf.hpp"

#include "Logger.hpp"
#include "Natives.hpp"
#include <Version.hpp>

#include <iostream>
#include <memory>

SAMPLOG_BEGIN_NS

/**
 * native ConsoleLogger(const name[])
 */
AMX_NATIVE(ConsoleLogger, 1)
	// Get params
	auto name = Logger::getString(amx, params[1]);

	// Logger
	auto console = spdlog::stdout_color_mt(name);

	// Return state
	return console ? 1 : 0;
AMX_NATIVE_END

/**
 * native BasicLogger(const name[], const filename[])
 */
AMX_NATIVE(BasicLogger, 2)
	// Get params
	auto name = Logger::getString(amx, params[1]);
	auto filename = Logger::getString(amx, params[2]);

	// Check path
	if (!Logger::checkPath(filename)) {
		logprintf("[SPDLog] Failed to create BasicLogger: %s.", filename.c_str());
		return false;
	}

	// Logger
	auto logger = spdlog::basic_logger_mt(name, filename);

	// Return state
	return logger ? 1 : 0;
AMX_NATIVE_END

/**
 * native RotatingLogger(const name[], const filename[], int seconds, int maxfiles)
 */
AMX_NATIVE(RotatingLogger, 4)
	// Get params
	auto name = Logger::getString(amx, params[1]);
	auto filename = Logger::getString(amx, params[2]);

	// Check path
	if (!Logger::checkPath(filename)) {
		logprintf("[SPDLog] Failed to create RotatingLogger: %s.", filename.c_str());
		return false;
	}

	// Logger
	auto logger = spdlog::rotating_logger_mt(name, filename, params[3], params[4]);
	
	// Return state
	return logger ? 1 : 0;
AMX_NATIVE_END

/**
 * native DailyLogger(const name[], const filename[], int hour, int minute)
 */
AMX_NATIVE(DailyLogger, 4)
	// Get params
	auto name = Logger::getString(amx, params[1]);
	auto filename = Logger::getString(amx, params[2]);

	// Check path
	if (!Logger::checkPath(filename)) {
		logprintf("[SPDLog] Failed to create DailyLogger: %s.", filename.c_str());
		return false;
	}

	// Logger
	auto logger = spdlog::daily_logger_mt(name, filename, params[3], params[4]);
	
	// Return state
	return logger ? 1 : 0;
AMX_NATIVE_END

/**
 * native SysLogger(const name[], const ident[], int pid)
 */
AMX_NATIVE(SysLogger, 3)
	// Get params
	auto name = Logger::getString(amx, params[1]);
	auto filename = Logger::getString(amx, params[2]);

	// Check path
	if (!Logger::checkPath(filename)) {
		logprintf("[SPDLog] Failed to create SysLogger: %s.", filename.c_str());
		return false;
	}

	#ifdef SPDLOG_ENABLE_SYSLOG 
		// Logger
		auto logger = spdlog::syslog_logger(name, ident, params[3]);

		// Return state
		return logger ? 1 : 0;
	#else
		return 0;
	#endif
AMX_NATIVE_END

/**
 * native LoggerSetAsyncMode(int size)
 */
AMX_NATIVE(LoggerSetAsyncMode, 1)
	// Get number from ptr
	auto number = static_cast<std::size_t>(params[1]);
if (number >= 1024) {
	spdlog::set_async_mode(number);
}
else {
	logprintf("[SPDLog] Error invalid size %d (LoggerSetAsyncMode)", number);
	return 0;
}
	return 1;
AMX_NATIVE_END

/**
 * native LoggerSetAsyncModeIntervaled(int size, int seconds)
 * Note flush interval just periodically flushes to disk, and not waits for the queue to become empty.
 */
AMX_NATIVE(LoggerSetAsyncModeIntervaled, 2)
	// Get number from ptr
	auto number = static_cast<std::size_t>(params[1]);
	auto seconds = static_cast<int>(params[2]);
	if (number >= 1024)
		spdlog::set_async_mode(number, spdlog::async_overflow_policy::block_retry, nullptr, std::chrono::seconds(seconds));
	else
		logprintf("[SPDLog] Error invalid size %d (LoggerSetAsyncModeIntervaled)", number);
	return 0;
AMX_NATIVE_END

/**
 * native LoggerSetPattern(int size)
 */
AMX_NATIVE(LoggerSetPattern, 1)
	// Get params
	auto pattern = Logger::getString(amx, params[1]);

	// Set pattern
	spdlog::set_pattern(pattern);
	return 0;
AMX_NATIVE_END

/**
 * native LoggerSetLevel(int size)
 * TODO: <Refactor> 
 */
AMX_NATIVE(LoggerSetLevel, 1)
	/**
	 *  trace = 0,
	 *	debug = 1,
	 *	info = 2,
	 *	warn = 3,
	 *	err = 4,
	 *	critical = 5,
	 *	off = 6
	 */

	// Default
	auto level = spdlog::level::trace;

	// Addr
	cell *pAddr = nullptr;
	amx_GetAddr(amx, params[1], &pAddr);

	// Get number from ptr
	auto number = static_cast<int>(*pAddr);

	// Translate log level
	switch (number) {
		case spdlog::level::trace:
			level = spdlog::level::trace;
			break;
		case spdlog::level::debug:
			level = spdlog::level::debug;
			break;
		case spdlog::level::info:
			level = spdlog::level::info;
			break;
		case spdlog::level::warn:
			level = spdlog::level::warn;
			break;
		case spdlog::level::err:
			level = spdlog::level::err;
			break;
		case spdlog::level::critical:
			level = spdlog::level::critical;
			break;
		case spdlog::level::off:
			level = spdlog::level::off;
			break;
	}

	// Set level
	spdlog::set_level(level);
	return 0;
AMX_NATIVE_END

/**
 * native LogInfo(const name[], const message[], {Float,_}:...)
 * Using message parsing from https://github.com/maddinat0r/samp-log/blob/master/src/natives.cpp
 */
AMX_NATIVE(LogInfo, 2)
	// Get params
	auto name = Logger::getString(amx, params[1]);

	// Get logger and write
	auto logger = spdlog::get(name);

	// We just parse the message if the logger is valid
	if (logger) {
		// Parse message
		fmt::MemoryWriter writer;
		auto state = Logger::formatString(name, amx, params, writer);

		// Write
		logger->info(writer.c_str());
		return state ? 1 : 0;
	}

	return 0;
AMX_NATIVE_END

/**
 * native LogWwarn(const name[], const message[], {Float,_}:...)
 */
AMX_NATIVE(LogWarn, 2)
	// Get params
	auto name = Logger::getString(amx, params[1]);

	// Get logger and write
	auto logger = spdlog::get(name);

	// We just parse the message if the logger is valid
	if (logger) {
		// Parse message
		fmt::MemoryWriter writer;
		auto state = Logger::formatString(name, amx, params, writer);

		// Write
		logger->warn(writer.c_str());
		return state ? 1 : 0;
	}

	return 0;
AMX_NATIVE_END

/**
 * native LogDebug(const name[], const message[], {Float,_}:...)
 */
AMX_NATIVE(LogDebug, 2)
	// Get params
	auto name = Logger::getString(amx, params[1]);

	// Get logger and write
	auto logger = spdlog::get(name);

	// We just parse the message if the logger is valid
	if (logger) {
		// Parse message
		fmt::MemoryWriter writer;
		auto state = Logger::formatString(name, amx, params, writer);

		// Write
		logger->debug(writer.c_str());
		return state ? 1 : 0;
	}

	return 0;
AMX_NATIVE_END

/**
 * native LogTrace(const name[], const message[], {Float,_}:...)
 */
AMX_NATIVE(LogTrace, 2)
	// Get params
	auto name = Logger::getString(amx, params[1]);

	// Get logger and write
	auto logger = spdlog::get(name);

	// We just parse the message if the logger is valid
	if (logger) {
		// Parse message
		fmt::MemoryWriter writer;
		auto state = Logger::formatString(name, amx, params, writer);

		// Write
		logger->trace(writer.c_str());
		return state ? 1 : 0;
	}

	return 0;
AMX_NATIVE_END

/**
 * native LogError(const name[], const message[], {Float,_}:...)
 */
AMX_NATIVE(LogError, 2)
	// Get params
	auto name = Logger::getString(amx, params[1]);

	// Get logger and write
	auto logger = spdlog::get(name);

	// We just parse the message if the logger is valid
	if (logger) {
		// Parse message
		fmt::MemoryWriter writer;
		auto state = Logger::formatString(name, amx, params, writer);

		// Write
		logger->error(writer.c_str());
		return state ? 1 : 0;
	}

	return 0;
AMX_NATIVE_END

/**
 * native LogCritical(const name[], const message[], {Float,_}:...)
 */
AMX_NATIVE(LogCritical, 2)
	// Get params
	auto name = Logger::getString(amx, params[1]);

	// Get logger and write
	auto logger = spdlog::get(name);

	// We just parse the message if the logger is valid
	if (logger) {
		// Parse message
		fmt::MemoryWriter writer;
		auto state = Logger::formatString(name, amx, params, writer);

		// Write
		logger->critical(writer.c_str());
		return state ? 1 : 0;
	}

	return 0;
AMX_NATIVE_END

/**
* native DropAllLogger()
*/
AMX_NATIVE(DropAllLogger, 0)
	spdlog::drop_all();
	return 0;
AMX_NATIVE_END

/**
 * natiive EnableErrorLogger()
 */
AMX_NATIVE(EnableErrorLogger, 0)
	spdlog::set_error_handler([](const std::string& msg) {
		logprintf("[SPDLog] Error: %s", msg.c_str());
	});
	return 0;
AMX_NATIVE_END

/**
* native LogFlushOn(const name[], int size)
*/
AMX_NATIVE(LogFlushOn, 2)
	// Get params
	auto name = Logger::getString(amx, params[1]);

	// Default
	auto level = spdlog::level::trace;

	// Addr
	cell *pAddr = nullptr;
	amx_GetAddr(amx, params[2], &pAddr);

	// Get number from ptr
	auto number = static_cast<int>(*pAddr);

	// Translate log level
	switch (number) {
	case spdlog::level::trace:
		level = spdlog::level::trace;
		break;
	case spdlog::level::debug:
		level = spdlog::level::debug;
		break;
	case spdlog::level::info:
		level = spdlog::level::info;
		break;
	case spdlog::level::warn:
		level = spdlog::level::warn;
		break;
	case spdlog::level::err:
		level = spdlog::level::err;
		break;
	case spdlog::level::critical:
		level = spdlog::level::critical;
		break;
	case spdlog::level::off:
		level = spdlog::level::off;
		break;
	}

	// Get logger and write
	auto logger = spdlog::get(name);
	if (logger)
		logger->flush_on(level);
	return logger ? 1 : 0;
AMX_NATIVE_END

/**
 * native LogFlush(const name[])
 */
AMX_NATIVE(LogFlush, 1) 
	// Get params
	auto name = Logger::getString(amx, params[1]);

	// Get logger and write
	auto logger = spdlog::get(name);
	if (logger)
		logger->flush();
	return logger ? 1 : 0;
AMX_NATIVE_END


/**
 * native LoggerVersion(const version[])
 */
AMX_NATIVE(LoggerVersion, 1)
	// Get params
	auto version = Logger::getString(amx, params[1]);

	// Check version
	if (version != BUILD_REV)
		logprintf("[SPDLog] Warning: Version mismatch detected %s != " BUILD_REV, version);

	return 1;
AMX_NATIVE_END
SAMPLOG_END_NS