# CMake required version
cmake_minimum_required(VERSION 3.1)

# "Only interpret if() arguments as variables or keywords when unquoted."
cmake_policy(SET CMP0054 NEW)

# Set cmake build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# Set Cpp11
set (CMAKE_CXX_STANDARD 11)

# Set project title
project(samp-plugin-spdlog)

# Add this to prevent missmatches
add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)

# Base path constants
set(SAMP_LOG_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(SAMP_LOG_BIN_ROOT ${CMAKE_CURRENT_BINARY_DIR})

# Externals
set(SAMP_LOG_EXTERNAL ${SAMP_LOG_ROOT}/external)

# Module
set(CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH}
	${SAMP_LOG_ROOT}/build/cmake
	${SAMP_LOG_EXTERNAL}/sampcmake
)

include(SpdlogExternal)

# Add src
add_subdirectory(src)
