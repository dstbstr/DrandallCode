project(ExtractorLibTest)

add_executable(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} gtest)
target_link_libraries(${PROJECT_NAME} gmock)
target_link_libraries(${PROJECT_NAME} ExtractorLib)

target_include_directories(${PROJECT_NAME} PRIVATE inc)
target_compile_options(${PROJECT_NAME} PRIVATE ${warnings})

target_sources(${PROJECT_NAME} PRIVATE 
	src/Main.cpp
	src/BodyCount.test.cpp
	src/CacheStore.test.cpp
	src/CommentExtractor.test.cpp
	src/DefineExtractor.test.cpp
	src/DefineFileParser.test.cpp
	src/FunctionDataExtractor.test.cpp
	src/IfDefExtractor.test.cpp
	src/IncludeMapGenerator.test.cpp
	src/LineFetcher.test.cpp
	src/NamespaceExtractor.test.cpp
	src/OperatorOverloadExtractor.test.cpp
	src/SpecialFunctionExtractor.test.cpp
	src/TypeDataExtractor.test.cpp
	src/VisibilityExtractor.test.cpp
)