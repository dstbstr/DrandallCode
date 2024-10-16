project(TodoLibTest)

add_executable(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} gtest)
target_link_libraries(${PROJECT_NAME} gmock)
target_link_libraries(${PROJECT_NAME} TodoLib)

target_include_directories(${PROJECT_NAME} PRIVATE inc)
target_compile_options(${PROJECT_NAME} PRIVATE ${warnings})

target_sources(${PROJECT_NAME} PRIVATE 
	src/Main.cpp
	src/TodoReader.test.cpp
)