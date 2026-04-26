cmake_minimum_required(VERSION 3.15)
project(world LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

enable_testing()

find_package(GTest CONFIG REQUIRED)
find_package(SDL3 REQUIRED)

add_library(core
    src/Foo.cpp
    src/Bar.cpp
)

target_include_directories(core PUBLIC src)

add_executable(app
    src/main.cpp
)

set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)

add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ASSETS_DIR}
    ${CMAKE_SOURCE_DIR}/bin/assets
)

target_link_libraries(app PRIVATE

    sdl::sdl
    core
)

target_link_options(app PRIVATE
    -static-libgcc
    -static-libstdc++
)

add_executable(app_tests
    tests/FooTest.cpp
    tests/BarTest.cpp
)

target_include_directories(app_tests PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(app_tests PRIVATE
    core
    gtest::gtest
    GTest::gtest_main
    GTest::gmock
)

include(GoogleTest)
include(CTest)
add_test(NAME app_tests COMMAND app_tests)
#gtest_discover_tests(app_tests)
