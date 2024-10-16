project(TodoManager)

add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} Core)
target_link_libraries(${PROJECT_NAME} TodoLib)
target_link_libraries(${PROJECT_NAME} CommandParser)

target_include_directories(${PROJECT_NAME} PRIVATE inc)
target_compile_options(${PROJECT_NAME} PRIVATE ${warnings})

target_sources(${PROJECT_NAME} PRIVATE
	src/TodoArgParse.cpp
	src/TodoManagerMain.cpp
)
