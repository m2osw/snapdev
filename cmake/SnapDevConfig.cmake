# - Try to find SnapDev
#
# Once done this will define
#
# SNAPDEV_FOUND        - System has SnapDev
# SNAPDEV_INCLUDE_DIR  - The SnapDev include directories
# SNAPDEV_LIBRARY      - The libraries needed to use SnapDev (none)
# SNAPDEV_DEFINITIONS  - Compiler switches required for using SnapDev (none)

find_path( SNAPDEV_INCLUDE_DIR snapdev/version.h
		   PATHS $ENV{SNAPDEV_INCLUDE_DIR}
		 )
#find_library( SNAPDEV_LIBRARY snapdev
#		   PATHS $ENV{SNAPDEV_LIBRARY}
#		 )
#mark_as_advanced( SNAPDEV_INCLUDE_DIR SNAPDEV_LIBRARY )
mark_as_advanced( SNAPDEV_INCLUDE_DIR )

set( SNAPDEV_INCLUDE_DIRS ${SNAPDEV_INCLUDE_DIR} )
#set( SNAPDEV_LIBRARIES    ${SNAPDEV_LIBRARY}     )

include( FindPackageHandleStandardArgs )
# handle the QUIETLY and REQUIRED arguments and set SNAPDEV_FOUND to TRUE
# if all listed variables are TRUE
#find_package_handle_standard_args( SnapDev DEFAULT_MSG SNAPDEV_INCLUDE_DIR SNAPDEV_LIBRARY )
find_package_handle_standard_args( SnapDev DEFAULT_MSG SNAPDEV_INCLUDE_DIR )
