cmake_minimum_required(VERSION 3.23)

project(
    CanpiServer
    VERSION 1.1.3
    DESCRIPTION "CANPiCAP/CANPiZero Interface"
    LANGUAGES CXX
)

# from "Professional CMake: A Practical Guide" by Craig Scott
if(DEFINED CMAKE_CXX_STANDARD)
    if(CMAKE_CXX_STANDARD EQUAL 98 OR
       CMAKE_CXX_STANDARD LESS 11)
        message(FATAL_ERROR
            "This project requires at least C++11"
        )
    endif()
else()
    set(CMAKE_CXX_STANDARD 11)
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
find_package(pigpio REQUIRED)
find_package(log4cpp REQUIRED)

# Define targets and sources
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

set(CPS_CONFIG  ${CMAKE_CURRENT_SOURCE_DIR}/config )
set(CPS_PKG     ${CMAKE_CURRENT_SOURCE_DIR}/package )
set(CPS_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/scripts )

add_custom_command(OUTPUT ${CPS_CONFIG}/canpi.cfg
    COMMAND python3 ${CPS_SCRIPTS}/create_canpi_cfg.py < ${CPS_CONFIG}/canpi-config-defn.json > ${CPS_CONFIG}/canpi.cfg
    DEPENDS ${CPS_CONFIG}/canpi-config-defn.json
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    VERBATIM
)

add_custom_target(canpi-cfg ALL
    DEPENDS ${CPS_CONFIG}/canpi.cfg
)

file(
    GLOB_RECURSE
    CONFIG_DEPS
    ${CPS_CONFIG}/*
)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.package
    COMMAND BFILE=$<TARGET_FILE:${CANPI_EXECUTABLE}> ODIR=${CPS_PKG} PKGNAME=canpi-server SDIR=${CMAKE_CURRENT_SOURCE_DIR} VERS=${PROJECT_VERSION} make -f ${CPS_PKG}/Makefile all && touch ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.package
    DEPENDS ${CPS_PKG}/canpi-server.epm $<TARGET_FILE:${CANPI_EXECUTABLE}> ${CPS_SCRIPTS}/canpidctl.bash ${CPS_PKG}/Makefile ${CONFIG_DEPS} ${CPS_CONFIG}/canpi.cfg
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# Building the package is a separate target
# that can be called from the command line
# with "make canpi-server-package"
# or "cmake --build build-debug --target canpi-server-package"
add_custom_target(canpi-server-package
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.package
)
