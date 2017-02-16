#ifndef SAMPLOG_NATIVES_HPP
#define SAMPLOG_NATIVES_HPP

#include "plugin.h"

SAMPLOG_BEGIN_NS

#define AMX_NATIVE(native) \
	cell AMX_NATIVE_CALL Natives::native(AMX *amx, cell *params)

#define AMX_DECLARE_NATIVE(native) \
	cell AMX_NATIVE_CALL native(AMX *amx, cell *params)

#define AMX_DEFINE_NATIVE(native) \
	{#native, SAMPLOG_NS::Natives::native},

namespace Natives 
{
	AMX_DECLARE_NATIVE(ConsoleLogger);
	AMX_DECLARE_NATIVE(BasicLogger);
	AMX_DECLARE_NATIVE(RotatingLogger);
	AMX_DECLARE_NATIVE(DailyLogger);
	AMX_DECLARE_NATIVE(SysLogger);

	AMX_DECLARE_NATIVE(LoggerSetAsyncMode);
	AMX_DECLARE_NATIVE(LoggerSetPattern);
	AMX_DECLARE_NATIVE(LoggerSetLevel);

	AMX_DECLARE_NATIVE(LogInfo);
	AMX_DECLARE_NATIVE(LogWarn);
	AMX_DECLARE_NATIVE(LogCritical);
	AMX_DECLARE_NATIVE(LogDebug);
	AMX_DECLARE_NATIVE(LogTrace);
	AMX_DECLARE_NATIVE(LogError);

	AMX_DECLARE_NATIVE(DropAllLogger);
	AMX_DECLARE_NATIVE(EnableErrorLogger);
};

SAMPLOG_END_NS
#endif