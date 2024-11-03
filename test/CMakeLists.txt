include(${CMAKE_SOURCE_DIR}/cmake/AddGTest.cmake)

enable_testing()

#Lexer
add_executable(
    lexerTest
    LexerTest.cpp
)

target_link_libraries(
    lexerTest
    GTest::gtest_main
    parser
)

target_include_directories(lexerTest PRIVATE ${CMAKE_SOURCE_DIR})

#Parser
add_executable(
    parserTest
    ParserTest.cpp
)

target_link_libraries(
    parserTest
    GTest::gtest_main
    parser
)


#DataStorage
add_executable(
    dataStorageTest
    DataStorageTest.cpp
)

target_link_libraries(
    dataStorageTest
    GTest::gtest_main
    parser
    dataStorage
)

#json
add_subdirectory(jsonTest)

include(GoogleTest)
gtest_discover_tests(lexerTest parserTest dataStorage jsonTest)
