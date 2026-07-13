find_package(GTest CONFIG REQUIRED)

add_executable(wfc_tests
    SudokuTest.cpp
)

target_include_directories(wfc_tests PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(wfc_tests PRIVATE
    wfc
    gtest::gtest
    GTest::gtest_main
    GTest::gmock
)

target_link_options(wfc_tests PRIVATE
    -static-libgcc
    -static-libstdc++
)

include(GoogleTest)
include(CTest)
add_test(NAME wfc_tests COMMAND wfc_tests)
#gtest_discover_tests(wfc_tests)
