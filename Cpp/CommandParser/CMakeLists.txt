project(CommandParser)

add_library(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} Core)
target_compile_options(${PROJECT_NAME} PRIVATE ${warnings})

target_include_directories(${PROJECT_NAME} PUBLIC inc)
target_sources(${PROJECT_NAME} PRIVATE 
	src/CommandSplitter.cpp
	src/Option.cpp
	src/OptionCollection.cpp
)
