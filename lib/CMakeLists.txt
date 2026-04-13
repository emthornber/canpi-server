# ---------------------------------------------------------------------------
# gpio-gpiod — libgpiod v2 implementation (Debian Trixie and later)
# ---------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.23)

project(
    gpio-gpiodcxx
    VERSION 0.1.0
    DESCRIPTION "GPIO implementation for Linux using libgpiod v2"
    LANGUAGES CXX
)

# Add STATIC library target for gpio-gpiodcxx
add_library(${PROJECT_NAME}
    STATIC
    ${PROJECT_SOURCE_DIR}/src/GpiodGpio.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    ${GPIODCXX_LINK_LIBRARIES}
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
    ${GPIODCXX_CFLAGS_OTHER}
)