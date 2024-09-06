cmake_minimum_required(VERSION 3.12)
project(smaster4s-timer
    VERSION 0.1.0.0)
add_executable(${PROJECT_NAME} src/main.c src/gui.c src/cli.c src/common.c)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)

include(FindPkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)
pkg_check_modules(SMASTER4S-INIS REQUIRED smaster4s-inis)

include_directories(${GTK4_INCLUDE_DIRS} ${SMASTER4S-INIS_INCLUDE_DIRS} res)
link_directories(${GTK4_LIBRARY_DIRS} ${SMASTER4S-INIS_LIBRARY_DIRS})
add_definitions(${GTK4_CFLAGS_OTHER})

add_compile_options(-O1)

target_link_libraries(${PROJECT_NAME} ${GTK4_LIBRARIES} ${SMASTER4S-INIS_LIBRARIES} m)
