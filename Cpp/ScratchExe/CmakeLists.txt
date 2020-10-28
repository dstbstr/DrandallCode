project(ScratchExe)

add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} ScratchLib)
target_link_libraries(${PROJECT_NAME} CommandParser)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${sources})
target_include_directories(${PROJECT_NAME} PRIVATE Inc)
