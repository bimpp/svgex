if(NOT DEFINED target_directory)
    message(FATAL_ERROR "target_directory not specified")
endif()

if(NOT DEFINED source_directory)
    message(FATAL_ERROR "source_directory not specified")
endif()

if(NOT DEFINED source_file)
    message(FATAL_ERROR "source_file not specified")
endif()

set(source_files ${source_file})

foreach(source_file ${source_files})
    set(source_path "${source_directory}/${source_file}")
    set(target_path "${target_directory}/${source_file}")
    message(STATUS "Copying if different ${source_path} to ${target_path}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different "${source_path}" "${target_path}"
        RESULT_VARIABLE result
        )
    if(result)
        message(FATAL_ERROR "Error copying a file - ${source_path}")
    endif()
endforeach()
