file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_project_library(
    engine
    SOURCES ${sources}
    DEPENDS ${Boost_LIBRARIES} graph actions tinyxml2
    )
