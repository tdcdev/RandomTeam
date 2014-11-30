# tinyxml2
file(GLOB tinyxml2src ${CMAKE_CURRENT_SOURCE_DIR}/tinyxml2/tinyxml2.cpp)
add_project_library(
    tinyxml2
    SOURCES ${tinyxml2src}
    DEST lib
    )
