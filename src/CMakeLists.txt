set(CODE_FILE_LIST
    ${BIMSVG_PATH_SRC_FILE_LIST}
    demo.cpp
    )
add_executable(demo ${CODE_FILE_LIST})

target_include_directories(demo PRIVATE ${BIMSVG_PATH_INCLUDE_LIST})

set_target_properties(demo PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIMSVG_PATH_OUTPUT_BIN})
