macro(list_directories directories)
    unset(${directories})
    file(GLOB tmp_list ${CMAKE_CURRENT_SOURCE_DIR}/*)
    foreach(tmp ${tmp_list})
        if(IS_DIRECTORY ${tmp})
            set(${directories} ${${directories}} ${tmp})
        endif()
    endforeach()
endmacro()

macro(add_project_library target)
    cmake_parse_arguments(LIBRARY "SHARED" "DEST" "SOURCES;DEPENDS" ${ARGN})
    if(LIBRARY_SHARED)
        add_library(${target} SHARED ${LIBRARY_SOURCES})
    else()
        add_library(${target} ${LIBRARY_SOURCES})
    endif()
    if(LIBRARY_DEPENDS)
        target_link_libraries(${target} ${LIBRARY_DEPENDS})
    endif()
    if(LIBRARY_DEST)
        install(
            TARGETS ${target}
            LIBRARY DESTINATION ${LIBRARY_DEST}
            ARCHIVE DESTINATION ${LIBRARY_DEST}
            )
    endif()
endmacro()

macro(add_project_executable target)
    cmake_parse_arguments(EXE "" "DEST" "SOURCES;DEPENDS" ${ARGN})
    add_executable(${target} ${EXE_SOURCES})
    if(EXE_DEPENDS)
        target_link_libraries(${target} ${EXE_DEPENDS})
    endif()
    if(EXE_DEST)
        install(TARGETS ${target} RUNTIME DESTINATION ${EXE_DEST})
    endif()
endmacro()
