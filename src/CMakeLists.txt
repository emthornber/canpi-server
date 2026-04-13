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

list(TRANSFORM CANPI_SOURCES REPLACE "[.]cpp" ".hpp" OUTPUT_VARIABLE CANPI_HEADERS)

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
    canpi_server_version.cpp canpi_server_version.hpp
)
target_include_directories(canpi_objs
    PRIVATE
    # where the library itself will look for its external headers
    ${CanpiServer_SOURCE_DIR}/lib/include
)

add_executable(${CANPI_EXECUTABLE} main.cpp)
target_link_libraries(${CANPI_EXECUTABLE}
    PRIVATE
    canpi_objs
    gpio-gpiodcxx
    gpiodcxx
    log4cpp
    pthread
    rt
)
