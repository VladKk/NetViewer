cmake_minimum_required(VERSION 3.17)
project(NetViewer LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "/home/vlad/Qt/5.14.2/gcc_64/lib/cmake")

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(NetViewer
        src/main.cpp
        src/main_win/main_win.cpp
        src/main_win/main_win.h
        src/main_win/main_win.ui)

target_link_libraries(NetViewer PRIVATE Qt5::Widgets)