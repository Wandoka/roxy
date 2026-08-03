cmake_minimum_required(VERSION 3.16)
project(roxy LANGUAGES C)

# Find ncurses
find_package(Curses REQUIRED)

# Collect all source and header files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

add_executable(roxy ${SOURCES} ${HEADERS})

target_include_directories(roxy PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CURSES_INCLUDE_DIRS}
)

target_link_libraries(roxy PRIVATE ${CURSES_LIBRARIES})

target_compile_options(roxy PRIVATE -Wall -Wextra)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)# для того чтобы LSP работал
