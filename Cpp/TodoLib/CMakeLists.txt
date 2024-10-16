project(TodoLib)

add_library(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} Core)
target_link_libraries(${PROJECT_NAME} OpenXLSX)

target_include_directories(${PROJECT_NAME} PUBLIC inc)
target_compile_options(${PROJECT_NAME} PRIVATE ${warnings})

target_sources(${PROJECT_NAME} PRIVATE 
	src/TodoReader.cpp
	src/TodoReport.cpp
	src/TodoWriter.cpp
)
