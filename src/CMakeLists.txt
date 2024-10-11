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

add_library(canpi_objs OBJECT ${CANPI_SOURCES} ${CANPI_HEADERS})

add_executable(${PROJECT_NAME} $<TARGET_OBJECTS:canpi_objs> main.cpp)
target_include_directories(${PROJECT_NAME}
    PRIVATE
    # where the executable itself will look for its internal headers
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(CMAKE_COMPILER_IS_GNUCXX)
    # compile options for this target only
    target_compile_options(${PROJECT_NAME} PRIVATE -c -Wall -fpermissive -mcpu=arm1176jzf-s)
endif()

target_link_libraries(${PROJECT_NAME} log4cpp pthread)
