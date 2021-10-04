project(IncludeCounter)

add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} Core)
target_link_libraries(${PROJECT_NAME} ExtractorLib)
target_link_libraries(${PROJECT_NAME} CommandParser)
target_link_libraries(${PROJECT_NAME} ReportGenerator)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "Src/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${sources})
target_include_directories(${PROJECT_NAME} PRIVATE Inc)
