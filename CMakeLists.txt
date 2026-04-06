cmake_minimum_required(VERSION 4.0)
project(wavplayer C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Wall -Werror")

add_executable(wavplayer 
    "src/wp_main.c"
    "src/wp_wav.c"
    "src/wp_player.c"
)

target_link_libraries(wavplayer asound)
