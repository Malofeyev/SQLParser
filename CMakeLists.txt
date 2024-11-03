cmake_minimum_required(VERSION 3.24)

project(selectParser)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard to conform to")
set(CMAKE_CXX_STANDARD_REQUIRED YES)

option(BUILD_TEST "Build yyparse tests" ON)
option(BUILD_APP  "Build the executable application" ON)

add_subdirectory(Expr)
add_subdirectory(Util)
add_subdirectory(DataStorage)
add_subdirectory(Json)
add_subdirectory(Parser)

if (BUILD_APP)
    add_subdirectory(App)
endif()

if (BUILD_TEST)
    add_subdirectory(test)
endif()
