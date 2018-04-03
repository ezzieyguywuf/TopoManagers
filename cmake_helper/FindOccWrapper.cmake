# Try to find OccWrapper
# Once done, this will define:
# OCCWRAPPER_FOUND - System has OccWrapper
# OCCWRAPPER_INCLUDE_DIRS - The OccWrapper include directories
# OCCWRAPPER_LIBRARIES - The libraries needed to use OccWrapper

set (OCCWRAPPER_FOUND "NO")

find_path(OCCWRAPPER_INCLUDE_DIR OccShape.h
    HINTS ${PROJECT_SOURCE_DIR}/../OccWrapper
    PATH_SUFFIXES include)

find_library(OCCWRAPPER_LIBRARY 
    NAMES OccShape 
    HINTS ${PROJECT_SOURCE_DIR}/../OccWrapper/build
    PATH_SUFFIXES lib)

get_filename_component(OCCWRAPPER_LIBRARIES_DIR ${OCCWRAPPER_LIBRARY} DIRECTORY CACHE)

include(FindPackageHandleStandardArgs) # to handle standard args, provided by cMake (?)
find_package_handle_standard_args(OccWrapper DEFAULT_MSG
                                  OCCWRAPPER_INCLUDE_DIR OCCWRAPPER_LIBRARY)
if (OCCWRAPPER_FOUND)
    message("-- Found OccWrapper")
    message("-- -- OccWrapper include directory = ${OCCWRAPPER_INCLUDE_DIR}")
    message("-- -- OccWrapper library directory = ${OCCWRAPPER_LIBRARIES_DIR}")
endif(OCCWRAPPER_FOUND)

mark_as_advanced(OCCWRAPPER_INCLUDE_DIR OCCWRAPPER_LIBRARY)
set(OCCWRAPPER_INCLUDE_DIRS ${OCCWRAPPER_INCLUDE_DIR})
set(OCCWRAPPER_LIBRARIES
    OccShape
    OccEdge
    OccFace
    OccSolid
    OccBooleanSolid
    OccBox
    OccCylinder
    OccSolidMaker
    OccModifiedSolid)
