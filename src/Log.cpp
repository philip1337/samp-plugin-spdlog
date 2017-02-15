#include "Config.hpp"

#include "plugin.h"

#include "Natives.hpp"
#include <map>

#include "Logprintf.hpp"
#include "Log.hpp"

SAMPLOG_BEGIN_NS

std::map<AMX*, Log*> loggers;

int Log::init()
{
	return amx_Register(m_amx, m_natives.get(), -1);
}

void Log::closeHandles()
{
}


void Log::registerInstance(AMX * amx, Log * log)
{
	loggers[amx] = log;
}

Log * Log::getInstance(AMX * amx)
{
	return loggers[amx];
}

SAMPLOG_END_NS