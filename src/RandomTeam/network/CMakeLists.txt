file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_project_library(
    network
    SOURCES ${sources}
    DEPENDS ${Boost_LIBRARIES}
    DEST lib
    )
