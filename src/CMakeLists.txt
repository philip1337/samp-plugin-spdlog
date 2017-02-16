include(AMXConfig)
include(AddSAMPPlugin)

set(SOURCE_FILES
	Logprintf.cpp
	Natives.cpp
	Plugin.cpp
)

SET(HEADER_FILES
	Logprintf.hpp
	Config.hpp
	Natives.hpp
)

add_samp_plugin(spdloglib
  ${SOURCE_FILES}
  ${HEADER_FILES}
  ${SAMPSDK_PATH}/amxplugin.cpp
  Plugin.def
)

target_link_libraries(spdloglib spdlog)

target_include_directories(spdloglib PUBLIC ${SAMPSDK_PATH})
target_include_directories(spdloglib PUBLIC ${SAMPSDK_PATH}/amx)

set_target_properties(spdloglib PROPERTIES OUTPUT_NAME "spdlog")
