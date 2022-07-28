project(SampleModuleTest)

set(SOURCE src/Main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME} gtest)
target_link_libraries(${PROJECT_NAME} gmock)
target_link_libraries(${PROJECT_NAME} Core)
target_link_libraries(${PROJECT_NAME} SampleModule)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp")
target_include_directories(${PROJECT_NAME} PRIVATE inc)
target_sources(${PROJECT_NAME} PRIVATE ${sources})