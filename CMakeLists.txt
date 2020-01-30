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

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR}
  translations/tasklog_ru.ts)
add_custom_target(translations ALL DEPENDS ${QM_FILES})

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(OTHER_FILES share/win.rc)
endif()

add_executable(tasklog WIN32 MACOSX_BUNDLE
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
  src/settingseditor.cpp
  src/settingseditor.h
  src/settings.cpp
  src/settings.h
  src/multicompletablelineedit.cpp
  src/multicompletablelineedit.h
  src/filewriter.cpp
  src/filewriter.h

  resources.qrc

  ${OTHER_FILES}
)

add_dependencies (tasklog translations)

target_link_libraries(tasklog PRIVATE Qt5::Widgets)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  find_package(Qt5 COMPONENTS X11Extras REQUIRED)
  find_package(X11 REQUIRED)
  target_link_libraries(tasklog PRIVATE Qt5::X11Extras X11)

  install(TARGETS tasklog DESTINATION /usr/bin)
  install(FILES share/tasklog.desktop DESTINATION /usr/share/applications)
  install(FILES share/book-512.png RENAME tasklog.png DESTINATION /usr/share/pixmaps/hicolor)
  install(FILES ${QM_FILES} DESTINATION /usr/translations)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  install(TARGETS tasklog DESTINATION /bin)
  install(FILES ${QM_FILES} DESTINATION /translations)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  FIND_LIBRARY(CARBON_LIBRARY Carbon)
  target_link_libraries(tasklog PRIVATE ${CARBON_LIBRARY})
  set(CMAKE_INSTALL_PREFIX tasklog.app)
  install(TARGETS ${APP_NAME} BUNDLE DESTINATION .)
  install(FILES ${QM_FILES} DESTINATION Contents/translations)
  set (MACOSX_BUNDLE_ICON_FILE icon.icns)
  install(FILES share/icon.icns DESTINATION Contents/Resources)
endif()
