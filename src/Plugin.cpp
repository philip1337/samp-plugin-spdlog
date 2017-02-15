#include "Config.hpp"
#include "plugin.h"
#include "Logprintf.hpp"
#include "Log.hpp"

void **ppPluginData;
extern void *pAMXFunctions;

// @PluginSupports
PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() {
   return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

// @PluginLoad
PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData) {
	// AMX Stuff
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];

	// Show loading message
	logprintf("   ----------------------------------------------");
	logprintf("    Initializing samp-plugin-log");
	logprintf("     https://github.com/Sphinxila/samp-plugin-log");
	logprintf("    Author: ProMetheus(Sphinxila)");
	logprintf("   ----------------------------------------------");
	return 1;
}

// @PluginUnload
PLUGIN_EXPORT void PLUGIN_CALL Unload() {
	logprintf("Plugin samp-plugin-log unloaded.");
}

// @AmxLoad
PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx) {

}

// @AmxUnload
PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx) {
	return AMX_ERR_NONE;
}