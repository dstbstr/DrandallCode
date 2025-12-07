cmake_minimum_required(VERSION 3.16)
project("Drandall Code")

include(FetchContent)

#################################
# Includes
#################################
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
)

FetchContent_Declare(
    OpenXlsx
    GIT_REPOSITORY https://github.com/troldal/openxlsx.git
    GIT_TAG master
)
FetchContent_Declare(CppCore GIT_REPOSITORY https://github.com/dstbstr/CppCore GIT_TAG origin/main)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
FetchContent_MakeAvailable(OpenXlsx)
FetchContent_MakeAvailable(CppCore)

#################################
# Drandall Code
#################################

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_definitions(-DGNU)
    else()
        add_definitions(-DCLANG)
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_definitions(-DMSVC)
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_definitions(-DDEBUG)
else()
    add_definitions(-DRELEASE)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(CommandParser)
add_subdirectory(ExtractorLib)
add_subdirectory(ReportGenerator)
add_subdirectory(TodoLib)

add_subdirectory(IncludeCounter)
add_subdirectory(TypeCounter)
add_subdirectory(TodoManager)

add_subdirectory(CommandParserTest)
add_subdirectory(ExtractorLibTest)
add_subdirectory(TodoLibTest)

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")
