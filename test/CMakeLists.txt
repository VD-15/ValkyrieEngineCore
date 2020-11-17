add_executable(ValkyrieEngineCoreTestDriver
	test.cpp)

add_subdirectory(EventBus)
add_subdirectory(ECS)

target_link_libraries(ValkyrieEngineCoreTestDriver
    PUBLIC
	ValkyrieEngineCore
	Catch2::Catch2)
#target_include_directories(ValkyrieEngineCoreTestDriver PUBLIC ${CMAKE_SOURCE_DIR}/deps/Catch2/single_include/catch2)

include(CTest)
include(${CMAKE_SOURCE_DIR}/deps/Catch2/contrib/Catch.cmake)
catch_discover_tests(ValkyrieEngineCoreTestDriver)
