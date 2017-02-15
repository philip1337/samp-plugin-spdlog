include(AMXConfig)
include(AddSAMPPlugin)

set(SOURCE_FILES
	Logprintf.cpp
	Log.cpp
	Natives.cpp
	Plugin.cpp
)

SET(HEADER_FILES
	Logprintf.hpp
	Config.hpp
	Log.hpp
	Natives.hpp
)

add_samp_plugin(loglib
  ${SOURCE_FILES}
  ${HEADER_FILES}
  ${SAMPSDK_PATH}/amxplugin.cpp
  plugin.def
)

target_link_libraries(loglib spdlog)

target_include_directories(loglib PUBLIC ${SAMPSDK_PATH})