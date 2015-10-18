project(gtk3shot C)

cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

add_executable(gtk3shot
    main.c
    gtk3shot.c
    gtk3shotwin.c
    gtk3shot-select.c
    )

target_link_libraries(gtk3shot ${GTK3_LIBRARIES})
