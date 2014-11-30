file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_project_executable(
    RandomTeam
    SOURCES ${sources}
    DEPENDS engine network ${BOOST_LIBRARIES}
    DEST bin
    )
