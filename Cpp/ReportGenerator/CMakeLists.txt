project(ReportGenerator)

add_library(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} Core)
target_link_libraries(${PROJECT_NAME} ExtractorLib)
target_link_libraries(${PROJECT_NAME} OpenXLSX::OpenXLSX)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${sources})
target_include_directories(${PROJECT_NAME} PUBLIC inc)
