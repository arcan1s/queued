#
# Copyright (c) 2017 Evgeniy Alekseev
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
# @file QueuedLibraries.cmake
# Additional file of queued library
# @author Evgeniy Alekseev
# @copyright MIT
# @bug https://github.com/arcan1s/queued/issues
##


##
# @brief Queued common libraries
##

find_package(Qt5 5.8.0 REQUIRED COMPONENTS Core DBus Network Sql)
##
# @brief add Qt definitions
##
add_definitions(
        ${Qt5Core_DEFINITIONS} ${Qt5DBus_DEFINITIONS} ${Qt5Network_DEFINITIONS}
        ${Qt5Sql_DEFINITIONS}
)
##
# @def Qt_INCLUDE
# Qt include paths
##
set(Qt_INCLUDE
        ${Qt5Core_INCLUDE_DIRS} ${Qt5DBus_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS}
        ${Qt5Sql_INCLUDE_DIRS}
)
##
# @def Qt_LIBRARIES
# Qt libraries
##
set(Qt_LIBRARIES
        ${Qt5Core_LIBRARIES} ${Qt5DBus_LIBRARIES} ${Qt5Network_LIBRARIES}
        ${Qt5Sql_LIBRARIES}
)
