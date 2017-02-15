/**
 * - SA-MP Log Plugin
 * - Author: ProMetheus (Sphinxila@Github)
 * - Description: SPDLog Port for SA-MP
 */

#include "Config.hpp"

// SAMP SDK & Log handler
#include "plugin.h"
#include "Logprintf.hpp"

// Natives
#include "Natives.hpp"

// SA-MP Stuff
void **ppPluginData;
extern void *pAMXFunctions;

// SPD LOG: https://github.com/gabime/spdlog/wiki/1.-QuickStart

// @PluginLoad
PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData) {
	// AMX Stuff
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];

	// Show loading message
	logprintf(" ----------------------------------------------");
	logprintf("  Initializing samp-plugin-spdlog (SPDLog Port)");
	logprintf("  https://github.com/Sphinxila/samp-plugin-spdlog");
	logprintf("  Author: ProMetheus(Sphinxila)");
	logprintf(" ----------------------------------------------");
	return 1;
}

// @PluginUnload
PLUGIN_EXPORT void PLUGIN_CALL Unload() {
	logprintf("Plugin samp-plugin-spdlog unloaded.");
}

// @Natives
const AMX_NATIVE_INFO natives[] = {
	// Create logger
	AMX_DEFINE_NATIVE(ConsoleLogger)	// Return string ID - (string name)
	AMX_DEFINE_NATIVE(BasicLogger)		// Return string ID - (string name, string path)
	AMX_DEFINE_NATIVE(RotatingLogger)	// Return string ID - (string name, string path, int time)
	AMX_DEFINE_NATIVE(DailyLogger)		// Return string ID - (string name, string path, int hour, int minute)
	AMX_DEFINE_NATIVE(SysLogger)		// Return string ID - (string name, string ident, int pid)
	
	// Logger specified functions
	//AMX_DEFINE_NATIVE(LoggerSetAsyncMode)	// Return void - (int size) 
	//AMX_DEFINE_NATIVE(LoggerSetPattern)		// Return void - (string pattern)
	//AMX_DEFINE_NATIVE(LoggerSetLevel)		// Return void - (int level)
	//AMX_DEFINE_NATIVE(LoggerSetFlushLevel)	// Return void - (int level)
	
	// Log functions
	//AMX_DEFINE_NATIVE(LogInfo)				// Return void - (int id, string, params...)
	//AMX_DEFINE_NATIVE(LogWarn)				// Return void - (int id, string, params...)
	//AMX_DEFINE_NATIVE(LogCritical)			// Return void - (int id, string, params...)
	//AMX_DEFINE_NATIVE(LogDebug)				// Return void - (int id, string, params...)
	
	// Error handler
	//AMX_DEFINE_NATIVE(SpdEnableErrorHandler)	// Return void - (bool state)
	//AMX_DEFINE_NATVE(SpdCloseAllLoggers)		// Return void - ()
	{NULL, NULL}
};

// @AmxLoad
PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx) {
	return amx_Register(amx, native_list, -1);
}

// @AmxUnload
PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx) {
	return AMX_ERR_NONE;
}

// @PluginSupports
PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() {
   return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}