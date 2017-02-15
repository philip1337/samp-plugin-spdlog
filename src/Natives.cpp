#include "Config.hpp"
#include "Logprintf.hpp"
#include "plugin.h"
#include "Natives.hpp"

#include <spdlog/spdlog.h>
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
	
	// Return name
	return console->name();
}

/**
 * native BasicLogger(const name[], const filename[])
 */
AMX_NATIVE(BasicLogger)
{
	// Get params
	char *filename, *logger;
	amx_StrParam(amx, params[1], logger);
	amx_StrParam(amx, params[2], filename);

	// Logger
	auto logger = spdlog::basic_logger_mt(logger, filename);
	
	// Return name
	return logger->name();
}

/**
 * native RotatingLogger(const name[], const filename[], int seconds)
 */
AMX_NATIVE(RotatingLogger)
{
	// Get params
	char *filename, *logger;
	amx_StrParam(amx, params[1], logger);
	amx_StrParam(amx, params[2], filename);
	
	// Logger
	auto logger = spdlog::basic_logger_mt(logger, filename, params[3]);
	
	// Return name
	return logger->name();
}

/**
 * native DailyLogger(const name[], const filename[], int hour, int minute)
 */
AMX_NATIVE(DailyLogger)
{
	// Get params
	char *filename, *logger;
	amx_StrParam(amx, params[1], logger);
	amx_StrParam(amx, params[2], filename);
	
	// Logger
	auto logger = spdlog::daily_logger_mt(logger, filename, params[3], params[4]);
	
	// Return name
	return logger->name();
}

/**
 * native SysLogger(const name[], const ident[], int pid)
 */
AMX_NATIVE(SysLogger)
{
	// Get params
	char *filename, *ident;
	amx_StrParam(amx, params[1], logger);
	amx_StrParam(amx, params[2], ident);
	
	// Logger
	auto logger = spdlog::syslog_logger(logger, ident, params[3]);
	
	// Return name
	return logger->name();
}


SAMPLOG_END_NS