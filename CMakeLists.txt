cmake_minimum_required(VERSION 3.23)

project(
    CanpiServer
    VERSION 2.0.2
    DESCRIPTION "CANPiCAP/CANPiZero Interface"
    LANGUAGES CXX
)

# from "Professional CMake: A Practical Guide" by Craig Scott
if(DEFINED CMAKE_CXX_STANDARD)
    if(CMAKE_CXX_STANDARD EQUAL 98 OR
       CMAKE_CXX_STANDARD LESS 17)
        message(FATAL_ERROR
            "This project requires at least C++17"
        )
    endif()
else()
    set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O1")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CANPI_EXECUTABLE canpid)

# from Earthly
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
find_package(gpiodcxx REQUIRED)
find_package(log4cpp REQUIRED)

# Define directories for config, package, and scripts
set(CPS_CONFIG  ${CanpiServer_SOURCE_DIR}/config )
set(CPS_PKG     ${CanpiServer_SOURCE_DIR}/package )
set(CPS_SCRIPTS ${CanpiServer_SOURCE_DIR}/scripts )

# Define targets and sources
add_subdirectory(config)
add_subdirectory(lib)
add_subdirectory(package)
add_subdirectory(src)

# enable testing and define tests
set(BUILD_TESTING OFF)
enable_testing()

if (BUILD_TESTING)
    # Add Google Test
    add_subdirectory(googletest)
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

    add_subdirectory(test)
endif(

)
# from Earthly
include(GNUInstallDirs)
install(TARGETS ${CANPI_EXECUTABLE} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
