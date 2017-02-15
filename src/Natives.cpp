#include "Config.hpp"
#include "Logprintf.hpp"
#include "plugin.h"
#include "Natives.hpp"

#include <spdlog/spdlog.h>
#include <iostream>
#include <memory>

SAMPLOG_BEGIN_NS

cell AMX_NATIVE_CALL CreateLog(AMX *amx, cell *params)
{
	// Get params
	char *filename, *logger;
	amx_StrParam(amx, params[1], logger);
	amx_StrParam(amx, params[2], filename);

	// Define path
	char maxpath[255];
	sprintf(maxpath, "scriptfiles/", filename);

	// Create logger
	auto my_logger = spdlog::basic_logger_mt(logger, maxpath);
	return 1;
}

cell AMX_NATIVE_CALL Log(AMX *amx, cell *params)
{

	const unsigned int
		first_param_idx = 2,
		num_args = (params[0] / sizeof(cell)),
		num_dyn_args = num_args - (first_param_idx - 1);
	unsigned int param_counter = 0;

	//auto log = Log::get

	logprintf("log call %d", num_dyn_args);
	return 1;
}


// Register natives
AMX_NATIVE_INFO natives[] =
{
	{ "CreateLog", CreateLog },
	{ "Log", Log }
};

// Getter for natives (TODO: Refactor this)
AMX_NATIVE_INFO * Natives::get()
{
	return natives;
}

SAMPLOG_END_NS