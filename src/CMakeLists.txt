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

# Plugin
add_samp_plugin(spdloglib
  ${SOURCE_FILES}
  ${HEADER_FILES}
  Plugin.def
)

# Definitions
target_compile_definitions(
	spdloglib
	PRIVATE
	BOOST_ALL_NO_LIB
)

# Link
target_link_libraries(spdloglib boost-lib-filesystem sampsdk spdlog)

# Includes
target_include_directories(spdloglib PRIVATE ${BOOST_LIB_PATH})

# Properties
set_target_properties(spdloglib PROPERTIES OUTPUT_NAME "spdlog")

# Target output
set_target_properties(spdloglib
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${SAMP_LOG_BIN_OUT_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${SAMP_LOG_BIN_OUT_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${SAMP_LOG_BIN_OUT_DIR}"
)

# SPDLog Include file handling
set(SPDLOG_INCLUDE_RAW "${SAMP_LOG_ROOT}/include/a_spdlog.inc.in")
set(SPDLOG_INCLUDE_SRC "${SAMP_LOG_BIN_ROOT}/a_spdlog.inc")
set(SPDLOG_INCLUDE_DST "${SAMP_LOG_BIN_OUT_DIR}/include/a_spdlog.inc")

# Parse include file to out
add_custom_command(
        TARGET  spdloglib PRE_BUILD
        COMMAND ${PYTHON_EXECUTABLE} ${SAMP_LOG_ROOT}/build/write_version.py ${SAMP_LOG_ROOT} ${SPDLOG_INCLUDE_RAW} ${SPDLOG_INCLUDE_SRC})

# Copy to output directorry (we also can write the file 
# directly but then we need to create also the directory tree)
add_custom_command(
        TARGET  spdloglib POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${SPDLOG_INCLUDE_SRC}
                ${SPDLOG_INCLUDE_DST})
