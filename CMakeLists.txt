cmake_minimum_required(VERSION 3.16)

set(CANPI_SERVER_VERSION 1.0.4)
if(DEFINED CMAKE_PROJECT_VERSION AND NOT "${CMAKE_PROJECT_VERSION}" STREQUAL "")
    set(CANPI_SERVER_VERSION ${CMAKE_PROJECT_VERSION})
endif()

project("canpi-server"
    VERSION ${CANPI_SERVER_VERSION}
    DESCRIPTION "CANPiCAP Interface"
    LANGUAGES CXX
)

message(STATUS "This project version: ${PROJECT_VERSION}")
message(STATUS "This project major version: ${PROJECT_VERSION_MAJOR}")

if (NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    message(STATUS "This project has a top-level one called [${CMAKE_PROJECT_NAME}]")
else()
    message(STATUS "This project is a top level one")
endif()

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
else()
    message(STATUS "C++ standard has been already set to: ${CMAKE_CXX_STANDARD}")
endif()

# from Earthly
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
find_package(log4cpp REQUIRED)

# Define targets and sources
add_subdirectory(src)

# enable testing and define tests
enable_testing()

# Add Google Test
add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_subdirectory(test)

# from Earthly
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

set(CPS_CONFIG  ${CMAKE_CURRENT_SOURCE_DIR}/config )
set(CPS_PKG     ${CMAKE_CURRENT_SOURCE_DIR}/package )
set(CPS_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/scripts )

add_custom_command(OUTPUT ${CPS_CONFIG}/canpi.cfg
    COMMAND python3 ${CPS_SCRIPTS}/create_canpi_cfg.py < ${CPS_CONFIG}/canpi-config-defn.json > ${CPS_CONFIG}/canpi.cfg
    DEPENDS ${CPS_CONFIG}/canpi-config-defn.json
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
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
    COMMAND BFILE=$<TARGET_FILE:${PROJECT_NAME}> ODIR=${CPS_PKG} SDIR=${CMAKE_CURRENT_SOURCE_DIR} VERS=${PROJECT_VERSION} make -f ${CPS_PKG}/Makefile all && touch ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.package
    DEPENDS ${CPS_PKG}/${PROJECT_NAME}.epm $<TARGET_FILE:${PROJECT_NAME}> ${CPS_SCRIPTS}/canpidctl.bash ${CONFIG_DEPS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(canpi-server-package ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.package
)
