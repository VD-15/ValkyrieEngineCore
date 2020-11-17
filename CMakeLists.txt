cmake_minimum_required(VERSION 3.10)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
	message(FATAL_ERROR "In-source builds are not allowed, please conduct builds from within a separate build directory")
endif()

set(CMAKE_DISABLE_IN_SOURCE_BUILD  ON)
set(CMAKE_DISABLE_SOURCE_CHANGES   ON)

project(ValkyrieEngineCore VERSION 0.2.0)

option(VLK_ENABLE_TRACE_LOGGING "Enable trace-level debug messages" OFF)
option(BUILD_TESTING "Build ValkyrieEngine tests" ON)

add_library(ValkyrieEngineCore
	${CMAKE_CURRENT_SOURCE_DIR}/include/ValkyrieEngine/ValkyrieEngine.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ValkyrieEngine.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Entity.cpp
)

target_compile_features(ValkyrieEngineCore PUBLIC cxx_std_20)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(ValkyrieEngineCore PUBLIC Threads::Threads)

target_include_directories(ValkyrieEngineCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (VLK_ENABLE_TRACE_LOGGING)
	target_compile_definitions(ValkyrieEngineCore PUBLIC VLK_ENABLE_TRACE_LOGGING)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(ValkyrieEngineCore PUBLIC VLK_DEBUG)
endif()

if (BUILD_TESTING)
    message(STATUS "Building tests...")
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/Catch2)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
    enable_testing()
endif()

