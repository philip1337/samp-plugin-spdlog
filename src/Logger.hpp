#ifndef SAMPLOG_LOGGER_HPP
#define SAMPLOG_LOGGER_HPP

#include "Config.hpp"

#include "plugin.h"
#include "Logprintf.hpp"

#include <spdlog/spdlog.h>

SAMPLOG_BEGIN_NS

class Logger {
public:
	/**
	 * Needed for parser https://github.com/maddinat0r/samp-log/blob/master/src/natives.cpp
	 */
	static const std::string getString(AMX *amx, cell param);

	/**
	 * Format string
	 */
	static bool Logger::formatString(const std::string loggerName, AMX *amx, cell *params, fmt::MemoryWriter &writer);
	
};

SAMPLOG_END_NS
#endif