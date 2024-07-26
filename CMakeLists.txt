cmake_minimum_required(VERSION 3.12)
project(smaster4s-timer 
    VERSION 0.1.0.0)
add_executable(${PROJECT_NAME} src/main.c src/gui.c src/cli.c)

include(FindPkgConfig)
pkg_check_modules(GTK4 REQUIRED gtk4)
include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})
add_definitions(${GTK4_CFLAGS_OTHER})
target_link_libraries(${PROJECT_NAME} ${GTK4_LIBRARIES})
