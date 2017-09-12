#
# Copyright (c) 2017 Queued team
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
#
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
##
# @file QueuedConfig.cmake
# Additional file of queued library
# @author Queued team
# @copyright MIT
# @bug https://github.com/arcan1s/queued/issues
##


##
# @brief try to find queued libraries
##

##
# @def QUEUED_DEFINITIONS
# optional cmake definitions
##
set(QUEUED_DEFINITIONS "")
##
# @def QUEUED_INCLUDE_DIRS
# path to root include directory
##
find_path(QUEUEDCORE_INCLUDE_DIR "queued/Queued.h" PATH_SUFFIXES "queued")
set(QUEUED_INCLUDE_DIRS "${QUEUEDCORE_INCLUDE_DIR}")
##
# @def QUEUED_LIBRARIES
# queued library name
##
find_library(QUEUEDCORE_LIBRARY NAMES "queued" "libqueued"
        PATH_SUFFIXES "queued")
set(QUEUED_LIBRARIES "${QUEUEDCORE_LIBRARY}")
##
# @def QUEUED_FOUND
# boolean, true if library has been found
##
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Queued DEFAULT_MSG
        QUEUED_LIBRARIES QUEUED_INCLUDE_DIRS)

##
# @brief additional defined macros
##
include("${CMAKE_CURRENT_LIST_DIR}/QueuedPaths.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/QueuedLibraries.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/QueuedMacros.cmake")
