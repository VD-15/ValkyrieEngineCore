target_sources(ValkyrieEngineCoreTestDriver PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/CompoundEventListener.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ManagedEventListener.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RawEventListener.cpp
	)

target_include_directories(ValkyrieEngineCoreTestDriver PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR})
