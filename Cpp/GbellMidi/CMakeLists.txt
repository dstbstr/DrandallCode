project(GBellMidi)

add_executable(${PROJECT_NAME})

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp")
target_link_libraries(${PROJECT_NAME} "Ws2_32.lib")
target_sources(${PROJECT_NAME} PRIVATE ${sources})
target_include_directories(${PROJECT_NAME} PRIVATE Inc)
