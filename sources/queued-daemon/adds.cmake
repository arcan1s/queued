# configure paths
configure_file ("QueuedPaths.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/QueuedPaths.cmake" @ONLY)
# additional files
install (FILES "QueuedConfig.cmake" DESTINATION "${LIB_INSTALL_DIR}/cmake/Queued")
install (FILES "QueuedLibraries.cmake" DESTINATION "${LIB_INSTALL_DIR}/cmake/Queued")
install (FILES "QueuedMacros.cmake" DESTINATION "${LIB_INSTALL_DIR}/cmake/Queued")
install (FILES "${CMAKE_CURRENT_BINARY_DIR}/QueuedPaths.cmake" DESTINATION "${LIB_INSTALL_DIR}/cmake/Queued")
