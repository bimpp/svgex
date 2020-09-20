if(NOT DEFINED target_directory)
    message(FATAL_ERROR "target_directory not specified")
endif()

if(NOT DEFINED source_directory)
    message(FATAL_ERROR "source_directory not specified")
endif()

set(source_file_paths)
file(GLOB_RECURSE h_source_files
    LIST_DIRECTORIES false
    RELATIVE ${source_directory}
    "${source_directory}/*.h")
set(source_file_paths ${source_file_paths} ${h_source_files})
file(GLOB_RECURSE hpp_source_files
    LIST_DIRECTORIES false
    RELATIVE ${source_directory}
    "${source_directory}/*.hpp")
set(source_file_paths ${source_file_paths} ${hpp_source_files})
file(GLOB_RECURSE inc_source_files
    LIST_DIRECTORIES false
    RELATIVE ${source_directory}
    "${source_directory}/*.inc")
set(source_file_paths ${source_file_paths} ${inc_source_files})

foreach(source_file_path ${source_file_paths})
    set(source_path "${source_directory}/${source_file_path}")
    set(target_path "${target_directory}/${source_file_path}")
    message(STATUS "Copying if different ${source_path} to ${target_path}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different "${source_path}" "${target_path}"
        RESULT_VARIABLE result
        )
    if(result)
        message(FATAL_ERROR "Error copying a file - ${source_path}")
    endif()
endforeach()
