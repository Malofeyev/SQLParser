add_library(parser Lexer.cpp Parser.cpp)
target_include_directories(parser PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(parser PRIVATE expr)

