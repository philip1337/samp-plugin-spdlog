#include "Config.hpp"
#include <cstdlib>
#include "Logprintf.hpp"
#include "plugin.h"

#include <spdlog/spdlog.h>

#include "Natives.hpp"

#include <iostream>
#include <memory>

SAMPLOG_BEGIN_NS

/**
 * native ConsoleLogger(const name[])
 */
AMX_NATIVE(ConsoleLogger)
{
	// Get params
	char *logger;
	amx_StrParam(amx, params[1], logger);

	// Logger
	auto console = spdlog::stdout_color_mt(logger);

	// Return state
	return logger ? 1 : 0;
}

/**
 * native BasicLogger(const name[], const filename[])
 */
AMX_NATIVE(BasicLogger)
{
	// Get params
	char *filename, *name;
	amx_StrParam(amx, params[1], name);
	amx_StrParam(amx, params[2], filename);

	// Logger
	auto logger = spdlog::basic_logger_mt(name, filename);

	// Return state
	return logger ? 1 : 0;
}

/**
 * native RotatingLogger(const name[], const filename[], int seconds, int maxfiles)
 */
AMX_NATIVE(RotatingLogger)
{
	// Get params
	char *filename, *name;
	amx_StrParam(amx, params[1], name);
	amx_StrParam(amx, params[2], filename);
	
	// Logger
	auto logger = spdlog::rotating_logger_mt(name, filename, params[3], params[4]);
	
	// Return state
	return logger ? 1 : 0;
}

/**
 * native DailyLogger(const name[], const filename[], int hour, int minute)
 */
AMX_NATIVE(DailyLogger)
{
	// Get params
	char *filename, *name;
	amx_StrParam(amx, params[1], name);
	amx_StrParam(amx, params[2], filename);
	
	// Logger
	auto logger = spdlog::daily_logger_mt(name, filename, params[3], params[4]);
	
	// Return state
	return logger ? 1 : 0;
}

/**
 * native SysLogger(const name[], const ident[], int pid)
 */
AMX_NATIVE(SysLogger)
{
	// Get params
	char *name, *ident;
	amx_StrParam(amx, params[1], name);
	amx_StrParam(amx, params[2], ident);

	#ifdef SPDLOG_ENABLE_SYSLOG 
		// Logger
		auto logger = spdlog::syslog_logger(name, ident, params[3]);

		// Return state
		return logger ? 1 : 0;
	#endif
	return 0;
}

/**
 * native LoggerSetAsyncMode(int size)
 */
AMX_NATIVE(LoggerSetAsyncMode)
{
	// Get number from ptr
	auto number = static_cast<std::size_t>(params[1]);
	if (number >= 1024)
		spdlog::set_async_mode(number);
	else
		logprintf("[SPDLog Error] Invalid size %d (LoggerSetAsyncMode)", number);
	return 0;
}

/**
 * native LoggerSetAsyncModeIntervaled(int size, int seconds)
 * Note flush interval just periodically flushes to disk, and not waits for the queue to become empty.
 */
AMX_NATIVE(LoggerSetAsyncModeIntervaled)
{
	// Get number from ptr
	auto number = static_cast<std::size_t>(params[1]);
	auto seconds = static_cast<int>(params[2]);
	if (number >= 1024)
		spdlog::set_async_mode(number, spdlog::async_overflow_policy::block_retry, nullptr, std::chrono::seconds(seconds));
	else
		logprintf("[SPDLog Error] Invalid size %d (LoggerSetAsyncModeIntervaled)", number);
	return 0;
}

/**
 * native LoggerSetPattern(int size)
 */
AMX_NATIVE(LoggerSetPattern)
{
	char *pattern;
	amx_StrParam(amx, params[1], pattern);

	// Set pattern
	spdlog::set_pattern(pattern);
	return 0;
}

/**
 * native LoggerSetLevel(int size)
 * TODO: <Refactor> 
 */
AMX_NATIVE(LoggerSetLevel)
{
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
}

/**
 * Needed for parser https://github.com/maddinat0r/samp-log/blob/master/src/natives.cpp
 */
std::string AMXAPI amx_GetCppString(AMX *amx, cell param)
{
	char *tmp;
	amx_StrParam(amx, param, tmp);
	return (tmp != NULL) ? tmp : std::string();
}

/**
 * native LogInfo(const name[], const message[], {Float,_}:...)
 * Using message parsing from https://github.com/maddinat0r/samp-log/blob/master/src/natives.cpp
 */
AMX_NATIVE(LogInfo)
{
	// Data
	std::string data;

	// Get params
	char *name;
	amx_StrParam(amx, params[1], name);

	// Message
	std::string format_str = amx_GetCppString(amx, params[2]);

	if (format_str.length() == 0)
		return 0;

	// Calculate param size
	const unsigned int first = 3, argc = (params[0] / sizeof(cell)), args = argc - (first - 1);

	fmt::MemoryWriter str_writer;

	unsigned int pCounter = 0;
	size_t
		spec_pos = std::string::npos,
		spec_offset = 0;

	while ((spec_pos = format_str.find('%', spec_offset)) != std::string::npos)
	{
		if (pCounter >= args)
			return false; // too many args given

		if (spec_pos == (format_str.length() - 1))
			return false;

		str_writer << format_str.substr(spec_offset, spec_pos - spec_offset);
		spec_offset = spec_pos + 2; // 2 = '%' + char specifier (like 'd' or 's')

		cell *pAddr = nullptr;

		switch (format_str.at(spec_pos + 1))
		{
		case '%': // '%' escape
			str_writer << '%';
			break;
		case 's': // string
			str_writer << amx_GetCppString(amx, params[first + pCounter]);
			break;
		case 'd': // decimal
		case 'i': // integer
			amx_GetAddr(amx, params[first + pCounter], &pAddr);
			str_writer << static_cast<int>(*pAddr);
			break;
		case 'f': // float
			amx_GetAddr(amx, params[first + pCounter], &pAddr);
			str_writer << amx_ctof(*pAddr);
			break;
		default:
			return false;
		}

		pCounter++;
	}

	// copy rest of format string
	str_writer << format_str.substr(spec_offset);

	// Get logger and write
	auto logger = spdlog::get(name);
	if (logger)
		logger->info(str_writer.c_str());
	return logger ? 1 : 0;
	return 0;
}

/**
 * native LogWwarn(const name[], const message[], {Float,_}:...)
 */
AMX_NATIVE(LogWarn)
{
	// Data
	std::string data;

	// Get params
	char *name;
	amx_StrParam(amx, params[1], name);

	// Message
	std::string format_str = amx_GetCppString(amx, params[2]);

	if (format_str.length() == 0)
		return 0;

	// Calculate param size
	const unsigned int first = 3, argc = (params[0] / sizeof(cell)), args = argc - (first - 1);

	fmt::MemoryWriter str_writer;

	unsigned int pCounter = 0;
	size_t
		spec_pos = std::string::npos,
		spec_offset = 0;

	while ((spec_pos = format_str.find('%', spec_offset)) != std::string::npos)
	{
		if (pCounter >= args)
			return false; // too many args given

		if (spec_pos == (format_str.length() - 1))
			return false;

		str_writer << format_str.substr(spec_offset, spec_pos - spec_offset);
		spec_offset = spec_pos + 2; // 2 = '%' + char specifier (like 'd' or 's')

		cell *pAddr = nullptr;

		switch (format_str.at(spec_pos + 1))
		{
		case '%': // '%' escape
			str_writer << '%';
			break;
		case 's': // string
			str_writer << amx_GetCppString(amx, params[first + pCounter]);
			break;
		case 'd': // decimal
		case 'i': // integer
			amx_GetAddr(amx, params[first + pCounter], &pAddr);
			str_writer << static_cast<int>(*pAddr);
			break;
		case 'f': // float
			amx_GetAddr(amx, params[first + pCounter], &pAddr);
			str_writer << amx_ctof(*pAddr);
			break;
		default:
			return false;
		}

		pCounter++;
	}

	// copy rest of format string
	str_writer << format_str.substr(spec_offset);

	// Get logger and write
	auto logger = spdlog::get(name);
	if (logger)
		logger->warn(str_writer.c_str());
	return logger ? 1 : 0;
}

/**
 * native LogDebug(const name[], const message[], {Float,_}:...)
 */
AMX_NATIVE(LogDebug)
{
	// Data
	std::string data;

	// Get params
	char *name;
	amx_StrParam(amx, params[1], name);

	// Message
	std::string format_str = amx_GetCppString(amx, params[2]);

	if (format_str.length() == 0)
		return 0;

	// Calculate param size
	const unsigned int first = 3, argc = (params[0] / sizeof(cell)), args = argc - (first - 1);

	fmt::MemoryWriter str_writer;

	unsigned int pCounter = 0;
	size_t
		spec_pos = std::string::npos,
		spec_offset = 0;

	while ((spec_pos = format_str.find('%', spec_offset)) != std::string::npos)
	{
		if (pCounter >= args)
			return false; // too many args given

		if (spec_pos == (format_str.length() - 1))
			return false;

		str_writer << format_str.substr(spec_offset, spec_pos - spec_offset);
		spec_offset = spec_pos + 2; // 2 = '%' + char specifier (like 'd' or 's')

		cell *pAddr = nullptr;

		switch (format_str.at(spec_pos + 1))
		{
		case '%': // '%' escape
			str_writer << '%';
			break;
		case 's': // string
			str_writer << amx_GetCppString(amx, params[first + pCounter]);
			break;
		case 'd': // decimal
		case 'i': // integer
			amx_GetAddr(amx, params[first + pCounter], &pAddr);
			str_writer << static_cast<int>(*pAddr);
			break;
		case 'f': // float
			amx_GetAddr(amx, params[first + pCounter], &pAddr);
			str_writer << amx_ctof(*pAddr);
			break;
		default:
			return false;
		}

		pCounter++;
	}

	// copy rest of format string
	str_writer << format_str.substr(spec_offset);

	// Get logger and write
	auto logger = spdlog::get(name);
	if (logger)
		logger->debug(str_writer.c_str());
	return logger ? 1 : 0;
}

/**
 * native LogTrace(const name[], const message[], {Float,_}:...)
 */
AMX_NATIVE(LogTrace)
{
	// Data
	std::string data;

	// Get params
	char *name;
	amx_StrParam(amx, params[1], name);

	// Message
	std::string format_str = amx_GetCppString(amx, params[2]);

	if (format_str.length() == 0)
		return 0;

	// Calculate param size
	const unsigned int first = 3, argc = (params[0] / sizeof(cell)), args = argc - (first - 1);

	fmt::MemoryWriter str_writer;

	unsigned int pCounter = 0;
	size_t
		spec_pos = std::string::npos,
		spec_offset = 0;

	while ((spec_pos = format_str.find('%', spec_offset)) != std::string::npos)
	{
		if (pCounter >= args)
			return false; // too many args given

		if (spec_pos == (format_str.length() - 1))
			return false;

		str_writer << format_str.substr(spec_offset, spec_pos - spec_offset);
		spec_offset = spec_pos + 2; // 2 = '%' + char specifier (like 'd' or 's')

		cell *pAddr = nullptr;

		switch (format_str.at(spec_pos + 1))
		{
		case '%': // '%' escape
			str_writer << '%';
			break;
		case 's': // string
			str_writer << amx_GetCppString(amx, params[first + pCounter]);
			break;
		case 'd': // decimal
		case 'i': // integer
			amx_GetAddr(amx, params[first + pCounter], &pAddr);
			str_writer << static_cast<int>(*pAddr);
			break;
		case 'f': // float
			amx_GetAddr(amx, params[first + pCounter], &pAddr);
			str_writer << amx_ctof(*pAddr);
			break;
		default:
			return false;
		}

		pCounter++;
	}

	// copy rest of format string
	str_writer << format_str.substr(spec_offset);

	// Get logger and write
	auto logger = spdlog::get(name);
	if (logger)
		logger->trace(str_writer.c_str());
	return logger ? 1 : 0;
}

/**
 * native LogError(const name[], const message[], {Float,_}:...)
 */
AMX_NATIVE(LogError)
{
	// Data
	std::string data;

	// Get params
	char *name;
	amx_StrParam(amx, params[1], name);

	// Message
	std::string format_str = amx_GetCppString(amx, params[2]);

	if (format_str.length() == 0)
		return 0;

	// Calculate param size
	const unsigned int first = 3, argc = (params[0] / sizeof(cell)), args = argc - (first - 1);

	fmt::MemoryWriter str_writer;

	unsigned int pCounter = 0;
	size_t
		spec_pos = std::string::npos,
		spec_offset = 0;

	while ((spec_pos = format_str.find('%', spec_offset)) != std::string::npos)
	{
		if (pCounter >= args)
			return false; // too many args given

		if (spec_pos == (format_str.length() - 1))
			return false;

		str_writer << format_str.substr(spec_offset, spec_pos - spec_offset);
		spec_offset = spec_pos + 2; // 2 = '%' + char specifier (like 'd' or 's')

		cell *pAddr = nullptr;

		switch (format_str.at(spec_pos + 1))
		{
		case '%': // '%' escape
			str_writer << '%';
			break;
		case 's': // string
			str_writer << amx_GetCppString(amx, params[first + pCounter]);
			break;
		case 'd': // decimal
		case 'i': // integer
			amx_GetAddr(amx, params[first + pCounter], &pAddr);
			str_writer << static_cast<int>(*pAddr);
			break;
		case 'f': // float
			amx_GetAddr(amx, params[first + pCounter], &pAddr);
			str_writer << amx_ctof(*pAddr);
			break;
		default:
			return false;
		}

		pCounter++;
	}

	// copy rest of format string
	str_writer << format_str.substr(spec_offset);

	// Get logger and write
	auto logger = spdlog::get(name);
	if (logger)
		logger->error(str_writer.c_str());
	return logger ? 1 : 0;
}

/**
 * native LogCritical(const name[], const message[], {Float,_}:...)
 */
AMX_NATIVE(LogCritical)
{
	// Data
	std::string data;

	// Get params
	char *name;
	amx_StrParam(amx, params[1], name);

	// Message
	std::string format_str = amx_GetCppString(amx, params[2]);

	if (format_str.length() == 0)
		return 0;

	// Calculate param size
	const unsigned int first = 3, argc = (params[0] / sizeof(cell)), args = argc - (first - 1);

	fmt::MemoryWriter str_writer;

	unsigned int pCounter = 0;
	size_t
		spec_pos = std::string::npos,
		spec_offset = 0;

	while ((spec_pos = format_str.find('%', spec_offset)) != std::string::npos)
	{
		if (pCounter >= args)
			return false; // too many args given

		if (spec_pos == (format_str.length() - 1))
			return false;

		str_writer << format_str.substr(spec_offset, spec_pos - spec_offset);
		spec_offset = spec_pos + 2; // 2 = '%' + char specifier (like 'd' or 's')

		cell *pAddr = nullptr;

		switch (format_str.at(spec_pos + 1))
		{
		case '%': // '%' escape
			str_writer << '%';
			break;
		case 's': // string
			str_writer << amx_GetCppString(amx, params[first + pCounter]);
			break;
		case 'd': // decimal
		case 'i': // integer
			amx_GetAddr(amx, params[first + pCounter], &pAddr);
			str_writer << static_cast<int>(*pAddr);
			break;
		case 'f': // float
			amx_GetAddr(amx, params[first + pCounter], &pAddr);
			str_writer << amx_ctof(*pAddr);
			break;
		default:
			return false;
		}

		pCounter++;
	}

	// copy rest of format string
	str_writer << format_str.substr(spec_offset);

	// Get logger and write
	auto logger = spdlog::get(name);
	if (logger)
		logger->critical(str_writer.c_str());
	return logger ? 1 : 0;
}

/**
* native DropAllLogger()
*/
AMX_NATIVE(DropAllLogger)
{
	spdlog::drop_all();
	return 0;
}

/**
 * natiive EnableErrorLogger()
 */
AMX_NATIVE(EnableErrorLogger)
{
	spdlog::set_error_handler([](const std::string& msg) {
		logprintf("[SPDLOG Error]: %s", msg.c_str());
	});
	return 0;
}

/**
* native LogFlushOn(const name[], int size)
*/
AMX_NATIVE(LogFlushOn)
{
	// Get params
	char *name;
	amx_StrParam(amx, params[1], name);

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
}

/**
 * native LogFlush(const name[])
 */
AMX_NATIVE(LogFlush) 
{
	// Get params
	char *name;
	amx_StrParam(amx, params[1], name);

	// Get logger and write
	auto logger = spdlog::get(name);
	if (logger)
		logger->flush();
	return logger ? 1 : 0;
}
SAMPLOG_END_NS