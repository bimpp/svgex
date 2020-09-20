set(BIMPPPLAN2D_PATH "${BIMPP_SVGEX_PATH_EXTERNALS}/plan2d")

find_path(BIMPPPLAN2D_PATH_INCLUDE "bimpp/plan2d.hpp" "${BIMPPPLAN2D_PATH}/include")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(BIMPPPLAN2D DEFAULT_MSG BIMPPPLAN2D_PATH_INCLUDE)

mark_as_advanced(BIMPPPLAN2D_PATH_INCLUDE)
