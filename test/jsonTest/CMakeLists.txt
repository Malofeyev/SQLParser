#json
add_executable(
    jsonTest
    JsonTest.cpp
)

target_link_libraries(
    jsonTest
    GTest::gtest_main
    parser
    json
    util
)

file(COPY json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
