# from Earthly but adjusted for libgpiod

include(FindPackageHandleStandardArgs)

# Try to find library
find_library(GPIOD_LIBRARY
    NAMES libgpiod.so
    PATHS ${CMAKE_SYSROOT}
    DOC "gpio library for C++"
)

# Try to find headers
find_path(GPIOD_INCLUDES
    NAMES gpiod.hpp
    PATHS ${CMAKE_SYSROOT}/usr/include/
    DOC "gpiod C++ header"
)

# Handle QUIET and REQUIRED and check the necessary variables
# were set and if so set ``GPIOD_FOUND``
find_package_handle_standard_args(gpiod
    REQUIRED_VARS GPIOD_LIBRARY GPIOD_INCLUDES
)

if (gpiod_FOUND)
    set(GPIOD_INCLUDE_DIR "${GPIOD_INCLUDES}")
    list(REMOVE_DUPLICATES GPIOD_INCLUDE_DIR)

    if (NOT TARGET gpiod::gpiod)
	add_library(gpiod::gpiod UNKNOWN IMPORTED)
	set_target_properties(gpiod::gpiod PROPERTIES
	    INTERFACE_INCLUDE_DIECTORIES "${GPIOD_INCLUDES}"
	    IMPORTED_LOCATION "${GPIOD_LIBRARY}")
    endif()
endif()

