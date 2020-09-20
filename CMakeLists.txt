cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(svgex)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_INSTALL_PREFIX "install" CACHE STRING "prefix")

set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
    ${CMAKE_MODULE_PATH}
    )
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_cmake")

set(BIMPP_SVGEX_PATH_ROOT ${CMAKE_SOURCE_DIR})
set(BIMPP_SVGEX_PATH_EXTERNALS ${CMAKE_SOURCE_DIR}/externals)
set(BIMPP_SVGEX_PATH_DOCS ${CMAKE_SOURCE_DIR}/docs)
set(BIMPP_SVGEX_PATH_INCLUDE ${CMAKE_SOURCE_DIR}/include)
set(BIMPP_SVGEX_PATH_SRC ${CMAKE_SOURCE_DIR}/src)
set(BIMPP_SVGEX_PATH_OUTPUT ${CMAKE_SOURCE_DIR}/output)
set(BIMPP_SVGEX_PATH_OUTPUT_INC ${BIMPP_SVGEX_PATH_OUTPUT}/include)
set(BIMPP_SVGEX_PATH_OUTPUT_BIN ${BIMPP_SVGEX_PATH_OUTPUT}/bin)
set(BIMPP_SVGEX_PATH_OUTPUT_LIB ${BIMPP_SVGEX_PATH_OUTPUT}/lib)

find_package(Boost REQUIRED)
find_package(RapidXMLNS REQUIRED)
find_package(SVGPP REQUIRED)
find_package(RapidJSON REQUIRED)

set(BIMPP_SVGEX_PATH_SRC_FILE_LIST
    ${BIMPP_SVGEX_PATH_INCLUDE}/bimpp/svgex.hpp
    )

add_custom_target(PACKAGE)

set_target_properties(PACKAGE PROPERTIES FOLDER "_cmake")

add_custom_command(TARGET PACKAGE
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    -D "target_directory:PATH=${BIMPP_SVGEX_PATH_OUTPUT_INC}"
    -D "source_directory:PATH=${BIMPP_SVGEX_PATH_INCLUDE}"
    -P "${CMAKE_SOURCE_DIR}/cmake/CopyDirectory.cmake"
    )

add_custom_command(TARGET PACKAGE
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    -D "target_directory:PATH=${BIMPP_SVGEX_PATH_OUTPUT_INC}"
    -D "source_directory:PATH=${SVGPP_PATH_INCLUDE}"
    -P "${CMAKE_SOURCE_DIR}/cmake/CopyDirectory.cmake"
    )

add_custom_command(TARGET PACKAGE
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    -D "target_directory:PATH=${BIMPP_SVGEX_PATH_OUTPUT_INC}"
    -D "source_directory:PATH=${RAPIDXMLNS_PATH_INCLUDE}"
    -D "source_file:FILE=rapidxml_ns.hpp"
    -P "${CMAKE_SOURCE_DIR}/cmake/CopyFile.cmake"
    )

add_custom_command(TARGET PACKAGE
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    -D "target_directory:PATH=${BIMPP_SVGEX_PATH_OUTPUT_INC}"
    -D "source_directory:PATH=${RAPIDJSON_PATH_INCLUDE}"
    -P "${CMAKE_SOURCE_DIR}/cmake/CopyDirectory.cmake"
    )

add_subdirectory(docs)
add_subdirectory(src)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT demo)
