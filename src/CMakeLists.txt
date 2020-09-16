set(CODE_FILE_LIST
    ${BIMSVG_PATH_INCLUDE}/bimsvg/bimsvg.hpp
    demo.cpp
    )
add_executable(demo ${CODE_FILE_LIST})

set(INCLUDE_DIR_LIST
    ${BIMSVG_PATH_INCLUDE}
    ${Boost_INCLUDE_DIR}
    ${RAPIDXMLNS_PATH_INCLUDE}
    ${RAPIDJSON_PATH_INCLUDE}
    ${SVGPP_PATH_INCLUDE}
    )
target_include_directories(demo PRIVATE ${INCLUDE_DIR_LIST})

set_target_properties(demo PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIMSVG_PATH_OUTPUT_BIN})
