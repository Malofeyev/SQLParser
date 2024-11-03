add_executable(selectParser main.cpp)

target_link_libraries(selectParser 
    PRIVATE parser 
    PRIVATE expr 
    PRIVATE dataStorage 
    PRIVATE util
)
target_include_directories(selectParser PRIVATE ${CMAKE_SOURCE_DIR})
