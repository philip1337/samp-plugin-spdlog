/**
 * - SA-MP Log Plugin
 * - Author: ProMetheus (Sphinxila@Github)
 * - Description: SPDLog Port for SA-MP
 */

#include "Config.hpp"

// SAMP SDK & Log handler
#include "plugin.h"
#include "Logprintf.hpp"

// SPDLog
#include <spdlog/spdlog.h>

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
	// Close all before unload
	spdlog::drop_all();

	// Unload message
	logprintf("Plugin samp-plugin-spdlog unloaded.");
}

// @Natives
const AMX_NATIVE_INFO natives[] = {
	// Create logger
	AMX_DEFINE_NATIVE(ConsoleLogger)	// Return int (state) - (string name)
	AMX_DEFINE_NATIVE(BasicLogger)		// Return int (state) - (string name, string path)
	AMX_DEFINE_NATIVE(RotatingLogger)	// Return int (state) - (string name, string path, int time, int maxfiles)
	AMX_DEFINE_NATIVE(DailyLogger)		// Return int (state) - (string name, string path, int hour, int minute)
	AMX_DEFINE_NATIVE(SysLogger)		// Return int (state) - (string name, string ident, int pid)
	
	// Logger specified functions
	AMX_DEFINE_NATIVE(LoggerSetAsyncMode)	// Return void - (int size) 
	AMX_DEFINE_NATIVE(LoggerSetAsyncModeIntervaled)	// Return void - (int size, int seconds) 
	AMX_DEFINE_NATIVE(LoggerSetPattern)		// Return void - (string pattern)
	AMX_DEFINE_NATIVE(LoggerSetLevel)		// Return void - (int level)
	
	// Log functions
	AMX_DEFINE_NATIVE(LogInfo)				// Return int - (string name, string message, params...)
	AMX_DEFINE_NATIVE(LogWarn)				// Return int - (string name, string message, params...)
	AMX_DEFINE_NATIVE(LogCritical)			// Return int - (string name, string message, params...)
	AMX_DEFINE_NATIVE(LogDebug)				// Return int - (string name, string message, params...)
	AMX_DEFINE_NATIVE(LogTrace)				// Return int - (string name, string message, params...)
	AMX_DEFINE_NATIVE(LogError)				// Return int - (string name, string message, params...)
	AMX_DEFINE_NATIVE(LogFlushOn)			// Return int - (string name, int level)
	AMX_DEFINE_NATIVE(LogFlush)				// Return int - (string name)

	// Error handler
	AMX_DEFINE_NATIVE(EnableErrorLogger)	// Return void - ()
	AMX_DEFINE_NATIVE(DropAllLogger)		// Return void - ()

	// Version check
	AMX_DEFINE_NATIVE(LoggerVersion)		// Return int - ()
	{NULL, NULL}
};

// @AmxLoad
PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx) {
	return amx_Register(amx, natives, -1);
}

// @AmxUnload
PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx) {
	return AMX_ERR_NONE;
}

// @PluginSupports
PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() {
   return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}