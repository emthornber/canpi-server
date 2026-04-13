# from Earthly but adjusted for libboost_filesystem

include(FindPackageHandleStandardArgs)

# Try to find library
find_library(BOOST_FS_LIBRARY
    NAMES libboost_filesystem.so
    PATHS ${CMAKE_SYSROOT}
    DOC "filesystem library for C++"
)

# Try to find headers
find_path(BOOST_FS__INCLUDES
    NAMES filesystem.hpp
    PATHS ${CMAKE_SYSROOT}/usr/include/boost/
    DOC "filesystem C++ header"
)

# Handle QUIET and REQUIRED and check the necessary variables
# were set and if so set ``BFS_FOUND``
find_package_handle_standard_args(bfs
    REQUIRED_VARS BOOST_FS_LIBRARY BOOST_FS__INCLUDES
)

if (bfs_FOUND)
    set(BOOST_FS_INCLUDE_DIR "${BOOST_FS__INCLUDES}")
    list(REMOVE_DUPLICATES BOOST_FS_INCLUDE_DIR)

    if (NOT TARGET boost::filesystem)
	add_library(boost::filesystem UNKNOWN IMPORTED)
	set_target_properties(boost::filesystem PROPERTIES
	    INTERFACE_INCLUDE_DIECTORIES "${BOOST_FS__INCLUDES}"
	    IMPORTED_LOCATION "${BOOST_FS_LIBRARY}")
    endif()
endif()

