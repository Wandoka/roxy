cmake_minimum_required(VERSION 3.16)
project(roxy LANGUAGES C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # для того чтобы LSP работал
find_library(NCURSESW_LIB ncursesw REQUIRED)

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

target_link_libraries(roxy PRIVATE ncursesw) # ncursesW W чтобы работали wide characters

target_compile_options(roxy PRIVATE -Wall -Wextra)

target_compile_definitions(roxy PRIVATE _XOPEN_SOURCE_EXTENDED=1) # Wide char для ncurses
