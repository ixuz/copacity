find_package(stb REQUIRED)

add_library(platform_stb
    ImageLoader.cpp
)

target_compile_options(platform_stb PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Werror
)

target_include_directories(platform_stb PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(platform_stb PRIVATE
    stb::stb
    core
)
