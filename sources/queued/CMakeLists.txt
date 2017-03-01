set (SUBPROJECT "queued")
message (STATUS "Subproject ${SUBPROJECT}")

configure_file ("QueuedConfig.h.in" "${CMAKE_BINARY_DIR}/QueuedConfig.h")

add_subdirectory ("src")
# headers
install (DIRECTORY "include/${SUBPROJECT}" DESTINATION "${INCLUDE_INSTALL_DIR}")
install (FILES "${CMAKE_BINARY_DIR}/QueuedConfig.h" DESTINATION "${INCLUDE_INSTALL_DIR}/${SUBPROJECT}")
# documentation
if (BUILD_DOCS)
    include ("docs.cmake")
endif ()
