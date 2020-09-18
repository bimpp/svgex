set(RAPIDXMLNS_PATH "${BIMPP_SVGEX_PATH_EXTERNALS}/rapidxml_ns")

find_path(RAPIDXMLNS_PATH_INCLUDE "rapidxml_ns.hpp" "${RAPIDXMLNS_PATH}")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(RAPIDXMLNS DEFAULT_MSG RAPIDXMLNS_PATH_INCLUDE)

mark_as_advanced(RAPIDXMLNS_PATH_INCLUDE)
