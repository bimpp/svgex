set(FILE_LIST)

set(FILE_LIST_ROOT
    ${BIMSVG_PATH_ROOT}/README.md
    #${BIMSVG_PATH_ROOT}/LICENSE
    )
source_group("root" FILES ${FILE_LIST_ROOT})
set(FILE_LIST ${FILE_LIST} ${FILE_LIST_ROOT})

set(FILE_LIST_DOC
    ${BIMSVG_PATH_DOCS}/usage.md
    )
source_group("docs" FILES ${FILE_LIST_DOC})
set(FILE_LIST ${FILE_LIST} ${FILE_LIST_DOC})

add_custom_target(DOCS SOURCES ${FILE_LIST})

set_target_properties(DOCS PROPERTIES FOLDER "_cmake")
