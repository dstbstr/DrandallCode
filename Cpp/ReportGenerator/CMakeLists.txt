project(ReportGenerator)

add_library(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} Core)
target_link_libraries(${PROJECT_NAME} ExtractorLib)
target_link_libraries(${PROJECT_NAME} OpenXLSX)

target_include_directories(${PROJECT_NAME} PUBLIC inc)
target_compile_options(${PROJECT_NAME} PRIVATE ${warnings})

target_sources(${PROJECT_NAME} PRIVATE
	src/CsvReport.cpp
	src/DetailedData.cpp
	src/DotReport.cpp
	src/DotToSvg.cpp
	src/ExcelReport.cpp
	src/SimpleData.cpp
)
