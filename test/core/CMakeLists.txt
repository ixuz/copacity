find_package(GTest CONFIG REQUIRED)

add_executable(core_tests
    FixedPointTest.cpp
    FooTest.cpp
    BarTest.cpp
)

target_include_directories(core_tests PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(core_tests PRIVATE
    core
    game
    gtest::gtest
    GTest::gtest_main
    GTest::gmock
)

target_link_options(core_tests PRIVATE
    -static-libgcc
    -static-libstdc++
)

include(GoogleTest)
include(CTest)
add_test(NAME core_tests COMMAND core_tests)
#gtest_discover_tests(core_tests)
