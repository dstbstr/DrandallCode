project(MidiLib)

add_library(${PROJECT_NAME})

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp")
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_link_libraries(${PROJECT_NAME} "Ws2_32.lib")
endif()

target_sources(${PROJECT_NAME} PRIVATE ${sources})
target_include_directories(${PROJECT_NAME} PUBLIC inc)
