add_library(util Util.cpp)

target_link_libraries(util 
    PRIVATE json
    PRIVATE expr
)

target_include_directories(util PUBLIC ${CMAKE_SOURCE_DIR})
