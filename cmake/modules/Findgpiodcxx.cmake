# from Earthly but adjusted for libgpiod

include(FindPackageHandleStandardArgs)

# Try to find library
find_library(GPIOD_CXX_LIBRARY
    NAMES libgpiodcxx.so
    PATHS ${CMAKE_SYSROOT}
    DOC "gpio library for C++"
)

# Try to find headers
find_path(GPIOD_CXX_INCLUDES
    NAMES gpiod.hpp
    PATHS ${CMAKE_SYSROOT}/usr/include/
    DOC "gpiod C++ header"
)

# Handle QUIET and REQUIRED and check the necessary variables
# were set and if so set ``GPIOD_FOUND``
find_package_handle_standard_args(gpiodcxx
    REQUIRED_VARS GPIOD_CXX_LIBRARY GPIOD_CXX_INCLUDES
)

if (gpiodcxx_FOUND)
    set(GPIOD_CXX_INCLUDE_DIR "${GPIOD_CXX_INCLUDES}")
    list(REMOVE_DUPLICATES GPIOD_CXX_INCLUDE_DIR)

    if (NOT TARGET gpiodcxx::gpiodcxx)
	add_library(gpiodcxx::gpiodcxx UNKNOWN IMPORTED)
	set_target_properties(gpiodcxx::gpiodcxx PROPERTIES
	    INTERFACE_INCLUDE_DIRECTORIES "${GPIOD_CXX_INCLUDES}"
	    IMPORTED_LOCATION "${GPIOD_CXX_LIBRARY}")
    endif()
endif()

