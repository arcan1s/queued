set (SUBPROJECT "plugins")
message (STATUS "Subproject ${SUBPROJECT}")

include_directories("${CMAKE_BINARY_DIR}"
        "${PROJECT_LIBRARY_DIR}/include" # required by moc
        "${PROJECT_TRDPARTY_DIR}")
find_package(Queued PATHS "${CMAKE_CURRENT_SOURCE_DIR}" NO_DEFAULT_PATH)

queued_install_plugin("emailnotify" "emailnotify" curl)
