cmake_minimum_required(VERSION 3.16)

set(CANPI_VERSION 1.0.4)
if(DEFINED CMAKE_PROJECT_VERSION AND NOT "${CMAKE_PROJECT_VERSION}" STREQUAL "")
    set(CANPI_VERSION ${CMAKE_PROJECT_VERSION})
endif()

project("canpi"
	VERSION ${CANPI_VERSION}
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

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
	PRIVATE
       	src/canHandler.cpp
        src/canHandler.h
        src/Client.cpp
        src/Client.h
        src/edSession.cpp
        src/edSession.h
        src/frameCAN.cpp
        src/frameCAN.h
        src/gpio.cpp
        src/gpio.h
        src/main.cpp
        src/msgdata.h
        src/nodeConfigurator.cpp
        src/nodeConfigurator.h
        src/opc_codes.cpp
        src/opc_codes.h
        src/opcodes.h
        src/sessionHandler.cpp
        src/sessionHandler.h
        src/tcpClient.cpp
        src/tcpClient.h
        src/tcpClientGridConnect.cpp
        src/tcpClientGridConnect.h
        src/tcpServer.cpp
        src/tcpServer.h
        src/Turnout.cpp
        src/Turnout.h
        src/utils.h)

target_include_directories(${PROJECT_NAME}
    PRIVATE
	# where the executable itself will look for its internal headers
	${CMAKE_CURRENT_SOURCE_DIR}/src
)
	
if(CMAKE_COMPILER_IS_GNUCXX)
    # compile options for this target only
    target_compile_options(${PROJECT_NAME} PRIVATE -c -Wall -fpermissive)
endif()

target_link_options(${PROJECT_NAME} BEFORE PRIVATE -Bdynamic)
target_link_libraries(${PROJECT_NAME} log4cpp pthread-2.31)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.deb
        COMMAND BFILE=$<TARGET_FILE:${PROJECT_NAME}> ODIR=${CMAKE_CURRENT_SOURCE_DIR}/package SDIR=${CMAKE_CURRENT_SOURCE_DIR} make -f ${CMAKE_CURRENT_SOURCE_DIR}/package/Makefile all && touch ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.package
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/package/${PROJECT_NAME}.epm $<TARGET_FILE:${PROJECT_NAME}>
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

add_custom_target(create_package ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.deb
)
