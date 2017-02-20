set (SUBPROJECT "queued")
message (STATUS "Subproject ${SUBPROJECT}")

add_subdirectory ("src")
# headers
install (DIRECTORY "include/${SUBPROJECT}" DESTINATION "${INCLUDE_INSTALL_DIR}")
install (FILES "${CMAKE_BINARY_DIR}/config.h" DESTINATION "${INCLUDE_INSTALL_DIR}/${SUBPROJECT}")
# documentation
if (BUILD_DOCS)
    include ("docs.cmake")
endif ()
