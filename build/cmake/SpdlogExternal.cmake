#################################################
# SPDLog lib
#################################################

add_subdirectory(${SAMP_LOG_EXTERNAL}/spdlog)

#################################################
# Boost lib settings
#################################################

if ("${BOOST_LIB_PATH}" STREQUAL "")
	# Set paths
	SET(BOOST_LIB_PATH ${SAMP_LOG_EXTERNAL}/boost)
endif()

SET(BOOST_SRC_LIB_PATH ${BOOST_LIB_PATH}/libs)

#################################
# System
#################################
SET(BOOST_SYS_FILES
	${BOOST_SRC_LIB_PATH}/system/src/error_code.cpp
)

add_library(
	boost-system-lib
	${BOOST_SYS_FILES}
)

target_include_directories(boost-system-lib PRIVATE ${BOOST_LIB_PATH})


#################################
# Filesystem lib
#################################
SET(BOOST_FS_FILES
	${BOOST_SRC_LIB_PATH}/filesystem/src/codecvt_error_category.cpp
	${BOOST_SRC_LIB_PATH}/filesystem/src/operations.cpp
	${BOOST_SRC_LIB_PATH}/filesystem/src/path.cpp
	${BOOST_SRC_LIB_PATH}/filesystem/src/path_traits.cpp
	${BOOST_SRC_LIB_PATH}/filesystem/src/portability.cpp
	${BOOST_SRC_LIB_PATH}/filesystem/src/unique_path.cpp
	${BOOST_SRC_LIB_PATH}/filesystem/src/utf8_codecvt_facet.cpp
	${BOOST_SRC_LIB_PATH}/filesystem/src/windows_file_codecvt.cpp
	${BOOST_SRC_LIB_PATH}/filesystem/src/windows_file_codecvt.hpp
)

add_library(
	boost-lib-filesystem
	${BOOST_FS_FILES}
)

target_compile_definitions(boost-lib-filesystem PRIVATE BOOST_ALL_NO_LIB)
target_include_directories(boost-lib-filesystem PRIVATE ${BOOST_LIB_PATH})

target_link_libraries(boost-lib-filesystem boost-system-lib)

##################################################
# AMXLib / sampsdk
##################################################
SET(SAMPSDK_PATH ${SAMP_LOG_EXTERNAL}/sampsdk)
set(SAMPSDK_INCLUDE_DIR ${SAMPSDK_PATH})

# Definitions
add_definitions(-DSAMPSDK_INCLUDE_DIR=${SAMPSDK_INCLUDE_DIR})
add_definitions(-DSAMPSDK_DIR=${SAMPSDK_PATH})

add_library(sampsdk 
	${SAMPSDK_PATH}/amxplugin.cpp
	${SAMPSDK_PATH}/plugin.h
	${SAMPSDK_PATH}/plugincommon.h
)

target_include_directories(sampsdk PUBLIC ${SAMPSDK_INCLUDE_DIR})
if (WIN32)
	target_compile_definitions(sampsdk PRIVATE WIN32)
else()
	target_include_directories(sampsdk PUBLIC ${SAMPSDK_INCLUDE_DIR}/amx)
	target_compile_definitions(sampsdk PRIVATE LINUX)
endif()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set_property(TARGET sampsdk APPEND_STRING PROPERTY COMPILE_FLAGS " -m32")
	set_property(TARGET sampsdk APPEND_STRING PROPERTY LINK_FLAGS    " -m32")
endif()