# main qt libraries
find_package(Qt5 5.8.0 REQUIRED COMPONENTS Core DBus Network Sql Test)
add_definitions(
        ${Qt5Core_DEFINITIONS} ${Qt5DBus_DEFINITIONS} ${Qt5Network_DEFINITIONS}
        ${Qt5Sql_DEFINITIONS}
)
set(Qt_INCLUDE
        ${Qt5Core_INCLUDE_DIRS} ${Qt5DBus_INCLUDE_DIRS} ${Qt5Network_INCLUDE_PATH}
        ${Qt5Sql_INCLUDE_DIRS}
)
set(Qt_LIBRARIES
        ${Qt5Core_LIBRARIES} ${Qt5DBus_LIBRARIES} ${Qt5Network_LIBRARIES}
        ${Qt5Sql_LIBRARIES}
)

