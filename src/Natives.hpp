#ifndef SAMPLOG_NATIVES_HPP
#define SAMPLOG_NATIVES_HPP

#include "plugin.h"

SAMPLOG_BEGIN_NS

#define AMX_NATIVE(native) \
	cell AMX_NATIVE_CALL Natives::native(AMX *amx, cell *params)

#define AMX_DECLARE_NATIVE(native) \
	cell AMX_NATIVE_CALL native(AMX *amx, cell *params)

#define AMX_DEFINE_NATIVE(native) \
	{#native, Native::native},

namespace Natives 
{
	AMX_DEFINE_NATIVE(ConsoleLogger);	
	AMX_DEFINE_NATIVE(BasicLogger);
	AMX_DEFINE_NATIVE(RotatingLogger);	
	AMX_DEFINE_NATIVE(DailyLogger);	
	AMX_DEFINE_NATIVE(SysLogger);
};

SAMPLOG_END_NS
#endif