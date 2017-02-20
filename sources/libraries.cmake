# main qt libraries
find_package(Qt5 5.6.0 REQUIRED COMPONENTS Core DBus Test)
add_definitions(
        ${Qt5Core_DEFINITIONS} ${Qt5DBus_DEFINITIONS}
)
set(Qt_INCLUDE
        ${Qt5Core_INCLUDE_DIRS} ${Qt5DBus_INCLUDE_DIRS}
)
set(Qt_LIBRARIES
        ${Qt5Core_LIBRARIES} ${Qt5DBus_LIBRARIES}
)

