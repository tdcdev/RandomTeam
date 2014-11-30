list_directories(directories)

foreach(dir ${directories})
    add_subdirectory(${dir})
endforeach()
