# Locate etherlab
#
# This module defines
#  ETHERLAB_FOUND, if false, do not try to link to etherlab
#  ETHERLAB_LIBRARY, where to find ethercat
#  ETHERLAB_INCLUDE_DIR, where to find ecrt.h
#
# If etherlab is not installed in a standard path, you can use the ETHERLAB_DIR CMake variable
# to tell CMake where yaml-cpp is.

# find the etherlab include directory
find_path(ETHERLAB_INCLUDE_DIR ecrt.h
          PATH_SUFFIXES include
          PATHS
          /usr/local/include/
          /usr/include/
          /opt/etherlab/
          ${ETHERLAB_DIR}/include/)

# find the etherlab library
find_library(ETHERCAT_LIBRARY
             NAMES ethercat
             PATH_SUFFIXES lib64 lib
             PATHS  /usr/local
                    /usr
                    /opt/etherlab
                    /opt
                    ${YAMLCPP_DIR}/lib)

find_library(ETHERCAT_LIBRARY_RTDM
             NAMES ethercat_rtdm
             PATH_SUFFIXES lib64 lib
             PATHS  /usr/local
                    /usr
                    /opt/etherlab
                    /opt
                    ${YAMLCPP_DIR}/lib)

# handle the QUIETLY and REQUIRED arguments and set YAMLCPP_FOUND to TRUE if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ETHERLAB DEFAULT_MSG ETHERLAB_INCLUDE_DIR ETHERLAB_LIBRARY)
mark_as_advanced(ETHERLAB_INCLUDE_DIR ETHERLAB_LIBRARY)