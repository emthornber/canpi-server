add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
    PRIVATE
    canHandler.cpp
    Client.cpp
    edSession.cpp
    frameCAN.cpp
    gpio.cpp
    main.cpp
    nodeConfigurator.cpp
    opc_codes.cpp
    sessionHandler.cpp
    tcpClient.cpp
    tcpClientGridConnect.cpp
    tcpServer.cpp
    Turnout.cpp
    PUBLIC
    canHandler.h
    Client.h
    edSession.h
    frameCAN.h
    gpio.h
    msgdata.h
    nodeConfigurator.h
    opc_codes.h
    opcodes.h
    sessionHandler.h
    tcpClient.h
    tcpClientGridConnect.h
    tcpServer.h
    Turnout.h
    utils.h
)

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
