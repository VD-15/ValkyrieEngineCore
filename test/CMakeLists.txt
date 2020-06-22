add_executable(ValkyrieEngineCoreTestDriver
	test.cpp)

add_subdirectory(EventBus)
add_subdirectory(Allocator)
add_subdirectory(ECS)
add_subdirectory(ThreadPool)

target_link_libraries(ValkyrieEngineCoreTestDriver
	ValkyrieEngineCore
	Catch2::Catch2
)

include(CTest)
include(${CMAKE_SOURCE_DIR}/deps/Catch2/contrib/Catch.cmake)
catch_discover_tests(ValkyrieEngineCoreTestDriver)
