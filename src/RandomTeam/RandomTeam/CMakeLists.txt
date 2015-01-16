add_project_executable(
    RandomTeam
    SOURCES RandomTeam tools.cpp
    DEPENDS engine network ${BOOST_LIBRARIES}
    DEST bin
    )

add_project_executable(
    TrueRandomTeam
    SOURCES TrueRandomTeam tools.cpp
    DEPENDS engine network ${BOOST_LIBRARIES}
    DEST bin
    )
