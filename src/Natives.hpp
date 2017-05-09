#ifndef SAMPLOG_NATIVES_HPP
#define SAMPLOG_NATIVES_HPP

#include "plugin.h"

SAMPLOG_BEGIN_NS

#define AMX_NATIVE(native, argc) \
	cell AMX_NATIVE_CALL Natives::native(AMX *amx, cell *params) { \
		AMX_CHECK_PARAMS(argc, native)

#define AMX_NATIVE_END }

#define AMX_DECLARE_NATIVE(native) \
	cell AMX_NATIVE_CALL native(AMX *amx, cell *params)

#define AMX_DEFINE_NATIVE(native) \
	{#native, SAMPLOG_NS::Natives::native},

#define AMX_CHECK_PARAMS(count, fname) \
	if (params[0] < (count * sizeof(cell))) \
	{ \
		logprintf("[SPDLog] Fatal Error calling '%s': expecting >= %d parameter(s), but found < %d", #fname, count, params[0] / sizeof(cell)); \
		return 0; \
	}

	namespace Natives
{
	AMX_DECLARE_NATIVE(ConsoleLogger);
	AMX_DECLARE_NATIVE(BasicLogger);
	AMX_DECLARE_NATIVE(RotatingLogger);
	AMX_DECLARE_NATIVE(DailyLogger);
	AMX_DECLARE_NATIVE(SysLogger);

	AMX_DECLARE_NATIVE(LoggerSetAsyncMode);
	AMX_DECLARE_NATIVE(LoggerSetAsyncModeIntervaled);
	AMX_DECLARE_NATIVE(LoggerSetPattern);
	AMX_DECLARE_NATIVE(LoggerSetLevel);

	AMX_DECLARE_NATIVE(LogInfo);
	AMX_DECLARE_NATIVE(LogWarn);
	AMX_DECLARE_NATIVE(LogCritical);
	AMX_DECLARE_NATIVE(LogDebug);
	AMX_DECLARE_NATIVE(LogTrace);
	AMX_DECLARE_NATIVE(LogError);
	AMX_DECLARE_NATIVE(LogFlushOn);
	AMX_DECLARE_NATIVE(LogFlush);
	AMX_DECLARE_NATIVE(LogLevel);

	AMX_DECLARE_NATIVE(DropAllLogger);
	AMX_DECLARE_NATIVE(EnableErrorLogger);

	AMX_DECLARE_NATIVE(LoggerVersion);
};

SAMPLOG_END_NS
#endif