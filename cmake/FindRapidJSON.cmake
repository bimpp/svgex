set(RAPIDJSON_PATH "${BIMPP_SVGEX_PATH_EXTERNALS}/rapidjson")

find_path(RAPIDJSON_PATH_INCLUDE "rapidjson/rapidjson.h" "${RAPIDJSON_PATH}/include")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(RAPIDJSON DEFAULT_MSG RAPIDJSON_PATH_INCLUDE)

mark_as_advanced(RAPIDJSON_PATH_INCLUDE)
