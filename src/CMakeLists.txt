set(CODE_FILE_LIST
    ${BIMPP_SVGEX_PATH_SRC_FILE_LIST}
    demo.cpp
    )
add_executable(demo ${CODE_FILE_LIST})

target_include_directories(demo PRIVATE ${BIMPP_SVGEX_PATH_INCLUDE_LIST})

set_target_properties(demo PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIMPP_SVGEX_PATH_OUTPUT_BIN})
