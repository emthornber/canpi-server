add_custom_target(canpi-cfg
    ALL
    COMMAND python3 ${CPS_SCRIPTS}/create_canpi_cfg.py < canpi-config-defn.json > canpi.cfg
    DEPENDS ${CPS_CONFIG}/canpi-config-defn.json ${CPS_SCRIPTS}/create_canpi_cfg.py
    BYPRODUCTS ${CPS_CONFIG}/canpi.cfg
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    VERBATIM
)

file(
    GLOB_RECURSE
    CONFIG_DEPS
    ${CPS_CONFIG}/*
)
list(REMOVE_ITEM CONFIG_DEPS ${CPS_CONFIG}/CMakeLists.txt)