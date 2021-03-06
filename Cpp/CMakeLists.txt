cmake_minimum_required(VERSION 3.14)
project("Cpp Scratch")
enable_testing()

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    #set(warnings "-Wall -Wextra -Werror")
    set(warnings "")
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_definitions(-DGNU)
    else()
        add_definitions(-DCLANG)
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    #set(warnings "/W4 /WX /EHsc")
    set(warnings "/W1 /EHsc")
    add_definitions(-DMSVC)
    #add_definitions(-D_REGEX_MAX_COMPLEXITY_COUNT=0) #remove max regex complexity, not sure if this would work outside of MSVC
endif()
if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
    set(CMAKE_C_FLAGS   "${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_definitions(-DDEBUG)
else()
    add_definitions(-DRELEASE)
endif()

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Core)
add_subdirectory(CommandParser)
add_subdirectory(ExtractorLib)

add_subdirectory(IncludeCounter)
add_subdirectory(TypeCounter)

add_subdirectory(CoreTest)
add_subdirectory(CommandParserTest)
add_subdirectory(ExtractorLibTest)

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")
