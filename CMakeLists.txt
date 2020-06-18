cmake_minimum_required(VERSION 3.10)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	message(FATAL_ERROR "In-source builds are not allowed, please conduct builds from within a separate build directory")
endif()

set(CMAKE_DISABLE_IN_SOURCE_BUILD  ON)
set(CMAKE_DISABLE_SOURCE_CHANGES   ON)
set(CMAKE_CXX_STANDARD             17)
set(CMAKE_CXX_STANDARD_REQUIRED    True)
set(CMAKE_CXX_FLAGS -pthread)

message(STATUS "Updating submodules")

execute_process(COMMAND git submodule update --init --recursive
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	RESULT_VARIABLE GIT_SUBMOD_RESULT)

if (NOT GIT_SUBMOD_RESULT EQUAL "0")
	message(FATAL_ERROR "Failed to update submodules, do you have git installed?")
endif()

project(ValkyrieEngineCore VERSION 0.1.0)

option(VLK_ENABLE_TRACE_LOGGING "Enable trace-level debug messages" OFF)
option(VLK_BUILD_TESTS "Build ValkyrieEngine test projects" ON)

add_library(ValkyrieEngineCore
	include/ValkyrieEngine/ValkyrieEngine.hpp
	src/ValkyrieEngine.cpp
	src/ThreadPool.cpp
)

target_include_directories(ValkyrieEngineCore PUBLIC ${CMAKE_SOURCE_DIR}/include)

if (VLK_ENABLE_TRACE_LOGGING)
	target_compile_definitions(ValkyrieEngineCore PUBLIC VLK_ENABLE_TRACE_LOGGING)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(ValkyrieEngineCore PUBLIC VLK_DEBUG)
endif()

if (VLK_BUILD_TESTS)
	add_subdirectory(${CMAKE_SOURCE_DIR}/deps/Catch2)
	add_subdirectory(test)
	enable_testing()
endif()

