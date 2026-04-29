set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)

add_executable(app
    main.cpp
)

add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ASSETS_DIR}
    ${CMAKE_SOURCE_DIR}/bin/assets
)

target_link_libraries(app PRIVATE
    core
    game
)

target_link_options(app PRIVATE
    -static-libgcc
    -static-libstdc++
)

target_compile_options(app PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Werror
)
