cmake_minimum_required(VERSION 3.5)

project(tasklog LANGUAGES CXX)

set (VERSION "0.1.0")
add_definitions(-DVERSION="${VERSION}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(tasklog
  src/constants.h
  src/debug.h
  src/main.cpp
  src/globalaction.cpp
  src/globalaction.h
  src/mainwindow.cpp
  src/mainwindow.h
  src/manager.cpp
  src/manager.h
  src/parser.cpp
  src/parser.h
  src/taskview.cpp
  src/taskview.h
  src/task.h
  src/taskmodel.cpp
  src/taskmodel.h
  src/trayicon.cpp
  src/trayicon.h

  resources.qrc
)

target_link_libraries(tasklog PRIVATE Qt5::Widgets libstdc++.so)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  find_package(Qt5 COMPONENTS X11Extras REQUIRED)
  find_package(X11 REQUIRED)
  target_link_libraries(tasklog PRIVATE Qt5::X11Extras X11)
endif()
