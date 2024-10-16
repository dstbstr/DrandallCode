project(ExtractorLib)

add_library(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} Core)

target_include_directories(${PROJECT_NAME} PUBLIC inc)
target_compile_options(${PROJECT_NAME} PRIVATE ${warnings})

target_sources(${PROJECT_NAME} PRIVATE 
	src/Cache/CachePurge.cpp
	src/Cache/CacheStore.cpp

	src/Extractors/FileDataExtractor.cpp
	src/Extractors/FunctionDataExtractor.cpp
	src/Extractors/TypeDataExtractor.cpp

	src/Private/BodyCount.cpp
	src/Private/CommentExtractor.cpp
	src/Private/DefineExtractor.cpp
	src/Private/IfDefExtractor.cpp
	src/Private/LineFetcher.cpp
	src/Private/NamespaceExtractor.cpp
	src/Private/VisibilityExtractor.cpp

	src/Workers/DefineFileParser.cpp
	src/Workers/FilePreProcessor.cpp
	src/Workers/IncludeMapGenerator.cpp
)
