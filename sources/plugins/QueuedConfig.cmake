set(QUEUED_DEFINITIONS "")
find_path(QUEUEDCORE_INCLUDE_DIR "queued/Queued.h" PATH_SUFFIXES "queued"
        PATHS "${PROJECT_LIBRARY_DIR}/include" NO_DEFAULT_PATH)
set(QUEUED_INCLUDE_DIRS "${QUEUEDCORE_INCLUDE_DIR}")
set(QUEUED_LIBRARIES "${PROJECT_LIBRARY}")
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Queued DEFAULT_MSG
        QUEUED_LIBRARIES QUEUED_INCLUDE_DIRS)

set(QUEUED_PLUGIN_ROOT "${DATA_INSTALL_DIR}/queued/plugins")

include("${PROJECT_LIBRARY_DIR}/QueuedLibraries.cmake")
include("${PROJECT_LIBRARY_DIR}/QueuedMacros.cmake")
