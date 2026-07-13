add_library(wfc
    Wfc.cpp
)

target_compile_options(wfc PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Werror
)

target_include_directories(wfc PUBLIC ${CMAKE_SOURCE_DIR}/src)
