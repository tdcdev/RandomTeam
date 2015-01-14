cmake_minimum_required(VERSION 2.8.9)

# setup project
project(RandomTeam)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()
set(VERSION_MAJOR 0)
set(VERSION_MINOR 3)
set(RESOURCES_DIR ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME})

# setup module path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# include macros
include(CMakeParseArguments)
include(CMakeMacros)

# setup CXX flags
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11")

# setup RPATH
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# setup version header
configure_file(
    ${CMAKE_SOURCE_DIR}/config/version.h.in
    ${CMAKE_BINARY_DIR}/include/${PROJECT_NAME}/version.h
    )

# setup definitions
if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_definitions(-DNDEBUG)
endif()
add_definitions(-DBOOST_ALL_DYN_LINK)

# find required libraries
find_package(Boost 1.55 REQUIRED system thread date_time program_options log)

# include directories
include_directories(${CMAKE_BINARY_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

# install
install(DIRECTORY ${CMAKE_BINARY_DIR}/include/ DESTINATION include)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/res/ DESTINATION ${RESOURCES_DIR})

# add subdirectories
add_subdirectory(${CMAKE_SOURCE_DIR}/src/${PROJECT_NAME})
add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty)
