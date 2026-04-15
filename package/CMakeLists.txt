add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.package
    COMMAND BFILE=$<TARGET_FILE:${CANPI_EXECUTABLE}> ODIR=${CPS_PKG} PKGNAME=canpi-server SDIR=${CanpiServer_SOURCE_DIR} VERS=${PROJECT_VERSION} make -f ${CPS_PKG}/Makefile all && touch ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.package
    DEPENDS ${CPS_PKG}/canpi-server.epm $<TARGET_FILE:${CANPI_EXECUTABLE}> ${CPS_SCRIPTS}/canpidctl.bash ${CPS_PKG}/Makefile ${CONFIG_DEPS} ${CPS_CONFIG}/canpi.cfg
    WORKING_DIRECTORY ${CanpiServer_SOURCE_DIR}
)

# Building the package is a separate target
# that can be called from the command line
# with "make canpi-server-package"
# or "cmake --build build-debug --target canpi-server-package"
add_custom_target(canpi-server-package
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.package
)
