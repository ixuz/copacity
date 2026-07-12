find_package(glm REQUIRED)

add_library(core
    Foo.cpp
    Bar.cpp
)

target_compile_options(core PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Werror
)

target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(core PRIVATE
    glm::glm
)
