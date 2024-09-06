# Add your test files
add_executable(canhandler_test
    canhandler_test.cpp
    ${CMAKE_SOURCE_DIR}/src/canHandler.cpp
    # ... other necessary source files ...
)

target_include_directories(canhandler_test
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(canhandler_test gtest gtest_main gmock)

add_test(NAME canhandler_test COMMAND canhandler_test)
