project(Core)

add_library(${PROJECT_NAME})

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${sources})
target_include_directories(${PROJECT_NAME} PUBLIC inc)
