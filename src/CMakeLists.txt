include(AMXConfig)
include(AddSAMPPlugin)

set(SOURCE_FILES
	Logprintf.cpp
	Natives.cpp
	Logger.cpp
	Plugin.cpp
)

SET(HEADER_FILES
	Logprintf.hpp
	Config.hpp
	Natives.hpp
	Logger.hpp
)

add_samp_plugin(spdloglib
  ${SOURCE_FILES}
  ${HEADER_FILES}
  Plugin.def
)

target_compile_definitions(
	spdloglib
	PRIVATE
	BOOST_ALL_NO_LIB
)

target_link_libraries(spdloglib boost-lib-filesystem sampsdk spdlog)
target_include_directories(spdloglib PRIVATE ${BOOST_LIB_PATH})

set_target_properties(spdloglib PROPERTIES OUTPUT_NAME "spdlog")
