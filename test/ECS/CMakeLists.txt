target_sources(ValkyrieEngineCoreTestDriver PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/ECS.cpp
)

target_include_directories(ValkyrieEngineCoreTestDriver PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
)
