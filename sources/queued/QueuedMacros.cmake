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
# @file QueuedMacros.cmake
# Additional file of queued library
# @author Queued team
# @copyright MIT
# @bug https://github.com/arcan1s/queued/issues
##


##
# @brief additional macros for queued
##

##
# @fn queued_install_plugin root [name] [libraries]
# @brief build and install plugin
# @param PLUGIN_ROOT         plugin root directory
# @param PLUGIN              plugin name (optional). Will be assigned from path
#                            if empty
# @param ADDS_LIBRARIES      additional libraries (optional)
#
##
macro(queued_install_plugin PLUGIN_ROOT)
    set(PLUGIN "${ARGV1}")
    if (NOT PLUGIN)
        set(PLUGIN "${PLUGIN_ROOT}")
    endif()
    set(ADDS_LIBRARIES "${ARGV2}")
    message (STATUS "Plugin ${PLUGIN}")
    # get sources
    file (GLOB_RECURSE ${PLUGIN}_SOURCES "${PLUGIN_ROOT}/*.cpp")
    file (GLOB_RECURSE ${PLUGIN}_HEADERS "${PLUGIN_ROOT}/*.h")
    qt5_wrap_cpp (${PLUGIN}_MOC_SOURCES "${${PLUGIN}_HEADERS}")

    # include directories
    include_directories ("${CMAKE_CURRENT_BINARY_DIR}"
                         "${Qt_INCLUDE}"
                         "${QUEUED_INCLUDE_DIRS}")

    # build
    add_library ("${PLUGIN}" MODULE "${${PLUGIN}_SOURCES}" "${${PLUGIN}_HEADERS}"
            "${${PLUGIN}_MOC_SOURCES}")
    target_link_libraries ("${PLUGIN}" "${QUEUED_LIBRARIES}" "${Qt_LIBRARIES}" "${ADDS_LIBRARIES}")

    # install
    install (TARGETS "${PLUGIN}" DESTINATION "${QUEUED_PLUGIN_ROOT}")
endmacro()
