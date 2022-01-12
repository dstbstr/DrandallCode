
#################################
# GTest
#################################
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.10.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


#########################
# ExtractorLibTest
#########################
project(ExtractorLibTest)

set(SOURCE src/Main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME} gtest)
target_link_libraries(${PROJECT_NAME} gmock)
target_link_libraries(${PROJECT_NAME} ExtractorLib)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp")
target_include_directories(${PROJECT_NAME} PRIVATE inc)
target_sources(${PROJECT_NAME} PRIVATE ${sources})