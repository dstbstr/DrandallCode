project(Core)

add_library(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC inc)

target_sources(${PROJECT_NAME} PRIVATE ${sources} 
	"src/Algorithms/AStar.test.cpp"
	"src/Algorithms/Shoelace.test.cpp"
	"src/Algorithms/ShuntingYard.test.cpp"

	"src/Constexpr/ConstexprAlgorithms.cpp"
	"src/Constexpr/ConstexprCollections.cpp"
	"src/Constexpr/ConstexprGeometry.cpp"
	"src/Constexpr/ConstexprMath.cpp"
	"src/Constexpr/ConstexprMatrix.cpp"
	"src/Constexpr/ConstexprStrUtils.cpp"

	"src/Instrumentation/Logging.cpp"
	"src/Instrumentation/ISink.cpp"
	"src/Instrumentation/LogWriter/StdOutLogWriter.cpp"

	"src/Platform/ExecuteCommand_win.cpp"

	"src/Utilities/ExpressionEvaluator.cpp"
	"src/Utilities/FileUtils.cpp"
	"src/Utilities/PathUtils.cpp"
	"src/Utilities/ProgressBar.cpp"
	"src/Utilities/StringUtils.cpp"
	"src/Utilities/TimeUtils.cpp"

	"src/BigInt.cpp"
)
