set(SVGPP_PATH "${BIMSVG_PATH_EXTERNALS}/svgpp")

find_path(SVGPP_PATH_INCLUDE "svgpp/svgpp.hpp" "${SVGPP_PATH}/include")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(SVGPP DEFAULT_MSG SVGPP_PATH_INCLUDE)

mark_as_advanced(SVGPP_PATH_INCLUDE)
