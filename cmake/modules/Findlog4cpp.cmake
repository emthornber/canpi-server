# from Earthly but adjusted for log4cpp

include(FindPackageHandleStandardArgs)

# Try to find library
find_library(LOG4CPP_LIBRARY
    NAMES liblog4cpp.so
    PATHS ${CMAKE_SYSROOT}
    DOC "Logging library for C++"
)

# Try to find headers
find_path(LOG4CPP_INCLUDES
    NAMES Category.hh
    PATHS ${CMAKE_SYSROOT}/usr/include/log4cpp/
    DOC "log4cpp headers"
)

# Handle QUIET and REQUIRED and check the necessary variables
# were set and if so set ``LOG4CPP_FOUND``
find_package_handle_standard_args(log4cpp
    REQUIRED_VARS LOG4CPP_LIBRARY LOG4CPP_INCLUDES
)

if (log4cpp_FOUND)
    set(LOG4CPP_INCLUDE_DIR "${LOG4CPP_INCLUDES}")
    list(REMOVE_DUPLICATES LOG4CPP_INCLUDE_DIR)

    if (NOT TARGET log4cpp::log4cpp)
	add_library(log4cpp::log4cpp UNKNOWN IMPORTED)
	set_target_properties(log4cpp::log4cpp PROPERTIES
	    INTERFACE_INCLUDE_DIECTORIES "${LOG4CPP_INCLUDES}"
	    IMPORTED_LOCATION "${LOG4CPP_LIBRARY}")
    endif()
endif()

