# set files
file (GLOB_RECURSE SUBPROJECT_SOURCES "*.cpp")
file (GLOB_RECURSE SUBPROJECT_HEADERS "*.h")

# include_path
include_directories ("${PROJECT_LIBRARY_DIR}/include"
        "${CMAKE_CURRENT_BINARY_DIR}"
        "${CMAKE_BINARY_DIR}"
        "${PROJECT_TRDPARTY_DIR}"
        "${Qt_INCLUDE}")

qt5_wrap_cpp (SUBPROJECT_MOC_SOURCES "${SUBPROJECT_HEADERS}")

add_executable ("${SUBPROJECT}" "${SUBPROJECT_HEADERS}" "${SUBPROJECT_SOURCES}"
        "${SUBPROJECT_MOC_SOURCES}")
target_link_libraries ("${SUBPROJECT}" "${PROJECT_LIBRARY}" "${Qt_LIBRARIES}")
# install properties
install (TARGETS "${SUBPROJECT}" DESTINATION "${BIN_INSTALL_DIR}")
