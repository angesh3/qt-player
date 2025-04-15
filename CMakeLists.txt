cmake_minimum_required(VERSION 3.16)
project(qt_youtube_player VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets Network Multimedia MultimediaWidgets OpenGLWidgets REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(MPV REQUIRED mpv)

# Add MPV library path
link_directories(${MPV_LIBRARY_DIRS})

add_executable(qt_youtube_player
    main.cpp
    mainwindow.cpp
    mainwindow.h
    videoplayer.cpp
    videoplayer.h
)

target_link_libraries(qt_youtube_player PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Multimedia
    Qt6::MultimediaWidgets
    Qt6::OpenGLWidgets
    ${MPV_LIBRARIES}
)

target_include_directories(qt_youtube_player PRIVATE ${MPV_INCLUDE_DIRS}) 