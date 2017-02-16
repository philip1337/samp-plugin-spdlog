#include "Config.hpp"
#include <cstdlib>
#include "Logprintf.hpp"
#include "plugin.h"

#include <spdlog/spdlog.h>

#include "Natives.hpp"

#include <iostream>
#include <memory>

SAMPLOG_BEGIN_NS

// TODO: Refactor
// http://stackoverflow.com/questions/39493542/building-a-dynamic-list-of-named-arguments-for-fmtlib
// helper only:
inline void set_type(fmt::ULongLong& result, uint32_t index, fmt::internal::Arg::Type t)
{
	unsigned shift = index * 4;
	uint64_t mask = 0xf;
	result |= static_cast<uint64_t>(t) << shift;
}

// input: 
//     pattern = fmt::format string
//     vars = dictionary of string/string arguments
// output:
//     formatted string
std::string dformat(const std::string& pattern, const std::unordered_map<std::string, std::string>& vars)
{
	// this is a vector of "named arguments" - straightforward enough.
	std::vector<fmt::internal::NamedArg<char>> av;

	// fmtlib uses an optimization that stores the types of the first 16 arguments as 
	// bitmask-encoded 64-bit integer. 
	fmt::ULongLong types = 0;

	// we need to build the named-arguments vector. 
	// we cannot resize it to the required size (even though we know it - we have the
	// dictionary), because NamedArg has no default constructor.
	uint32_t index = 0;
	for (const auto& item : vars)
	{
		av.emplace_back(fmt::internal::NamedArg<char>(item.first, item.second));

		// we need to pack the first 16 arguments - see above
		if (index < fmt::ArgList::MAX_PACKED_ARGS)
		{
			set_type(types, index, fmt::internal::Arg::NAMED_ARG);
		}
		++index;
	}

	// and this is a bit tricky: depending on the number of arguments we use two mutually
	// incompatible vectors to create an arglist. It has everything to do with the speed
	// (and memory) optimization above, even though the code looks nearly identical.
	if (index >= fmt::ArgList::MAX_PACKED_ARGS)
	{
		std::vector<fmt::internal::Arg> avdata;

		// note the additional terminating Arg::NONE
		avdata.resize(vars.size() + 1);
		index = 0;
		for (const auto& item : av)
		{
			avdata[index].type = fmt::internal::Arg::NAMED_ARG;
			avdata[index].pointer = &av[index];
			++index;
		}
		return fmt::format(pattern, fmt::ArgList(types, &avdata[0]));
	}
	else
	{
		std::vector<fmt::internal::Value> avdata;

		// no need for terminating Arg::NONE, because ARG_NONE is the last encoded type 
		avdata.resize(vars.size());
		index = 0;
		for (const auto& item : av)
		{
			avdata[index].pointer = &av[index];
			++index;
		}
		return fmt::format(pattern, fmt::ArgList(types, &avdata[0]));
	}
}

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
	// Addr
	cell *pAddr = nullptr;
	amx_GetAddr(amx, params[1], &pAddr);

	// Get number from ptr
	auto number = static_cast<int>(*pAddr);

	spdlog::set_async_mode((size_t)number);
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

std::string AMXAPI amx_GetCppString(AMX *amx, cell param)
{
	char *tmp;
	amx_StrParam(amx, param, tmp);
	return (tmp != NULL) ? tmp : std::string();
}

/**
 * native LogInfo(const name[], const message[], {Float,_}:...)
 */
AMX_NATIVE(LogInfo)
{
	// Data
	std::string data;

	// Get params
	char *name, *message;
	amx_StrParam(amx, params[1], name);
	amx_StrParam(amx, params[2], message);

	if (name == NULL || message == NULL)
		return 0;

	// Calculate param size
	const unsigned int first = 3, argc = (params[0] / sizeof(cell)), args = argc - (first - 1);

	// Dirty solution
	int counter = 0;
	std::unordered_map<std::string, std::string> vars;
	for (unsigned int i = first; i <= argc; i++) {
		// Addr
		cell *pAddr = nullptr;
		amx_GetAddr(amx, params[i], &pAddr);
		auto number = static_cast<int>(*pAddr);
		auto data = amx_GetCppString(amx, params[i]);

		// Integer
		if (data.length() == 1 && number > 1) {
			vars[std::to_string(counter)] = std::to_string(number);
		} else {
			vars[std::to_string(counter)] = data;
		}
		counter++;
	}

	// Convert to string
	std::string str(message);

	// Get logger and write
	spdlog::get(name)->info(dformat(str,vars).c_str());
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
	char *name, *message;
	amx_StrParam(amx, params[1], name);
	amx_StrParam(amx, params[2], message);

	if (name == NULL || message == NULL)
		return 0;

	// Calculate param size
	const unsigned int first = 3, argc = (params[0] / sizeof(cell)), args = argc - (first - 1);

	// Dirty solution
	int counter = 0;
	std::unordered_map<std::string, std::string> vars;
	for (unsigned int i = first; i <= argc; i++) {
		// Addr
		cell *pAddr = nullptr;
		amx_GetAddr(amx, params[i], &pAddr);
		auto number = static_cast<int>(*pAddr);
		auto data = amx_GetCppString(amx, params[i]);

		// Integer
		if (data.length() == 1 && number > 1) {
			vars[std::to_string(counter)] = std::to_string(number);
		}
		else {
			vars[std::to_string(counter)] = data;
		}
		counter++;
	}

	// Convert to string
	std::string str(message);

	// Get logger and write
	spdlog::get(name)->warn(dformat(str, vars).c_str());
	return 0;
}

/**
 * native LogDebug(const name[], const message[], {Float,_}:...)
 */
AMX_NATIVE(LogDebug)
{
	// Data
	std::string data;

	// Get params
	char *name, *message;
	amx_StrParam(amx, params[1], name);
	amx_StrParam(amx, params[2], message);

	if (name == NULL || message == NULL)
		return 0;

	// Calculate param size
	const unsigned int first = 3, argc = (params[0] / sizeof(cell)), args = argc - (first - 1);

	// Dirty solution
	int counter = 0;
	std::unordered_map<std::string, std::string> vars;
	for (unsigned int i = first; i <= argc; i++) {
		// Addr
		cell *pAddr = nullptr;
		amx_GetAddr(amx, params[i], &pAddr);
		auto number = static_cast<int>(*pAddr);
		auto data = amx_GetCppString(amx, params[i]);

		// Integer
		if (data.length() == 1 && number > 1) {
			vars[std::to_string(counter)] = std::to_string(number);
		}
		else {
			vars[std::to_string(counter)] = data;
		}
		counter++;
	}

	// Convert to string
	std::string str(message);

	// Get logger and write
	spdlog::get(name)->debug(dformat(str, vars).c_str());
	return 0;
}

/**
 * native LogTrace(const name[], const message[], {Float,_}:...)
 */
AMX_NATIVE(LogTrace)
{
	// Data
	std::string data;

	// Get params
	char *name, *message;
	amx_StrParam(amx, params[1], name);
	amx_StrParam(amx, params[2], message);

	if (name == NULL || message == NULL)
		return 0;

	// Calculate param size
	const unsigned int first = 3, argc = (params[0] / sizeof(cell)), args = argc - (first - 1);

	// Dirty solution
	int counter = 0;
	std::unordered_map<std::string, std::string> vars;
	for (unsigned int i = first; i <= argc; i++) {
		// Addr
		cell *pAddr = nullptr;
		amx_GetAddr(amx, params[i], &pAddr);
		auto number = static_cast<int>(*pAddr);
		auto data = amx_GetCppString(amx, params[i]);

		// Integer
		if (data.length() == 1 && number > 1) {
			vars[std::to_string(counter)] = std::to_string(number);
		}
		else {
			vars[std::to_string(counter)] = data;
		}
		counter++;
	}

	// Convert to string
	std::string str(message);

	// Get logger and write
	spdlog::get(name)->trace(dformat(str, vars).c_str());
	return 0;
}

/**
 * native LogError(const name[], const message[], {Float,_}:...)
 */
AMX_NATIVE(LogError)
{
	// Data
	std::string data;

	// Get params
	char *name, *message;
	amx_StrParam(amx, params[1], name);
	amx_StrParam(amx, params[2], message);

	if (name == NULL || message == NULL)
		return 0;

	// Calculate param size
	const unsigned int first = 3, argc = (params[0] / sizeof(cell)), args = argc - (first - 1);

	// Dirty solution
	int counter = 0;
	std::unordered_map<std::string, std::string> vars;
	for (unsigned int i = first; i <= argc; i++) {
		// Addr
		cell *pAddr = nullptr;
		amx_GetAddr(amx, params[i], &pAddr);
		auto number = static_cast<int>(*pAddr);
		auto data = amx_GetCppString(amx, params[i]);

		// Integer
		if (data.length() == 1 && number > 1) {
			vars[std::to_string(counter)] = std::to_string(number);
		}
		else {
			vars[std::to_string(counter)] = data;
		}
		counter++;
	}

	// Convert to string
	std::string str(message);

	// Get logger and write
	spdlog::get(name)->error(dformat(str, vars).c_str());
	return 0;
}

/**
 * native LogCritical(const name[], const message[], {Float,_}:...)
 */
AMX_NATIVE(LogCritical)
{
	// Data
	std::string data;

	// Get params
	char *name, *message;
	amx_StrParam(amx, params[1], name);
	amx_StrParam(amx, params[2], message);

	if (name == NULL || message == NULL)
		return 0;

	// Calculate param size
	const unsigned int first = 3, argc = (params[0] / sizeof(cell)), args = argc - (first - 1);

	// Dirty solution
	int counter = 0;
	std::unordered_map<std::string, std::string> vars;
	for (unsigned int i = first; i <= argc; i++) {
		// Addr
		cell *pAddr = nullptr;
		amx_GetAddr(amx, params[i], &pAddr);
		auto number = static_cast<int>(*pAddr);
		auto data = amx_GetCppString(amx, params[i]);

		// Integer
		if (data.length() == 1 && number > 1) {
			vars[std::to_string(counter)] = std::to_string(number);
		}
		else {
			vars[std::to_string(counter)] = data;
		}
		counter++;
	}

	// Convert to string
	std::string str(message);

	// Get logger and write
	spdlog::get(name)->critical(dformat(str, vars).c_str());
	return 0;
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

SAMPLOG_END_NS