file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_project_library(
    graph
    SOURCES ${sources}
    DEST lib
    )
