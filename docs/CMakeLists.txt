set(FILE_LIST)

set(FILE_LIST_ROOT
    ${BIMPP_SVGEX_PATH_ROOT}/README.md
    ${BIMPP_SVGEX_PATH_ROOT}/LICENSE
    )
source_group("root" FILES ${FILE_LIST_ROOT})
set(FILE_LIST ${FILE_LIST} ${FILE_LIST_ROOT})

set(FILE_LIST_ROOT
    ${BIMPP_SVGEX_PATH_ROOT}/docs/Doxyfile
    )
source_group("docs" FILES ${FILE_LIST_ROOT})
set(FILE_LIST ${FILE_LIST} ${FILE_LIST_ROOT})

add_custom_target(DOCS SOURCES ${FILE_LIST})

set_target_properties(DOCS PROPERTIES FOLDER "_cmake")
