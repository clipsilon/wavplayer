cmake_minimum_required(VERSION 4.0)
project(linux-audio C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Wall -Werror")

add_executable(linux-audio 
    "src/la_main.c"
    "src/la_wav.c"
    "src/la_player.c"
)

target_link_libraries(linux-audio m asound)
