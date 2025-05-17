set(CANPI_SOURCES
    canHandler.cpp
    Client.cpp
    edSession.cpp
    frameCAN.cpp
    gpio.cpp
    nodeConfigurator.cpp
    opc_codes.cpp
    sessionHandler.cpp
    tcpClient.cpp
    tcpClientGridConnect.cpp
    tcpServer.cpp
    Turnout.cpp
)

list(TRANSFORM CANPI_SOURCES REPLACE "[.]cpp" ".h" OUTPUT_VARIABLE CANPI_HEADERS)

# from "Professional CMake: A Practical Guide" by Craig Scott"
configure_file(
    canpi_server_version.cpp.in
    ${CMAKE_CURRENT_SOURCE_DIR}/canpi_server_version.cpp
    @ONLY
)

add_library(
    canpi_objs
    OBJECT
    ${CANPI_SOURCES} ${CANPI_HEADERS}
    canpi_server_version.cpp canpi_server_version.h
)

add_executable(${CANPI_EXECUTABLE} $<TARGET_OBJECTS:canpi_objs> main.cpp)
target_include_directories(${CANPI_EXECUTABLE}
    PRIVATE
    # where the executable itself will look for its internal headers
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${CANPI_EXECUTABLE} log4cpp pthread pigpiod_if2 rt)
