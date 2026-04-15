add_custom_command(OUTPUT canpi.cfg
    COMMAND python3 ${CPS_SCRIPTS}/create_canpi_cfg.py < canpi-config-defn.json > canpi.cfg
    DEPENDS canpi-config-defn.json
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
list(REMOVE_ITEM CONFIG_DEPS ${CPS_CONFIG}/CMakeLists.txt)