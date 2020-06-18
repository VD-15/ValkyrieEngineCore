target_sources(ValkyrieEngineCoreTestDriver PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/ArrayAllocator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ChunkAllocator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LinkedAllocator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SingletonAllocator.cpp
)

target_include_directories(ValkyrieEngineCoreTestDriver PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
)
