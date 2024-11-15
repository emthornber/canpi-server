# from Earthly but adjusted for pigpio

include(FindPackageHandleStandardArgs)

# Try to find library
find_library(PIGPIO_LIBRARY
    NAMES libpigpio.so
    PATHS ${CMAKE_SYSROOT}
    DOC "Logging library for C++"
)

# Try to find headers
find_path(PIGPIO_INCLUDES
    NAMES pigpio.h
    PATHS ${CMAKE_SYSROOT}/usr/include/
    DOC "pigpio header"
)

# Handle QUIET and REQUIRED and check the necessary variables
# were set and if so set ``PIGPIO_FOUND``
find_package_handle_standard_args(pigpio
    REQUIRED_VARS PIGPIO_LIBRARY PIGPIO_INCLUDES
)

if (pigpio_FOUND)
    set(PIGPIO_INCLUDE_DIR "${PIGPIO_INCLUDES}")
    list(REMOVE_DUPLICATES PIGPIO_INCLUDE_DIR)

    if (NOT TARGET pigpio::pigpio)
	add_library(pigpio::pigpio UNKNOWN IMPORTED)
	set_target_properties(pigpio::pigpio PROPERTIES
	    INTERFACE_INCLUDE_DIECTORIES "${PIGPIO_INCLUDES}"
	    IMPORTED_LOCATION "${PIGPIO_LIBRARY}")
    endif()
endif()

