cmake_minimum_required(VERSION 3.5)

project(SSevGui LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE CODE_FILES *.cpp)

SET(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/zh_CN.ts
)

find_package(Qt5LinguistTools)
qt5_create_translation(QM_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${TS_FILES} OPTIONS -source-language en_US -no-obsolete)
message(${QM_FILES})


find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Qt5 COMPONENTS DBus REQUIRED)

add_executable(SSevGui
  main.cpp
  MainWindow.cpp
  connection.cpp
  connectionitem.cpp
  connectiontablemodel.cpp
  statusnotifier.cpp
  ConnectionInfoWgt.cpp
  Settings.cpp
  SubscriptionManagerWgt.cpp
  BusView.cpp
  DataDefine.cpp
  ConnectionTest.cpp
  SSevGUI.qrc
  Tools.cpp
  ShareDialog.cpp
  ${QM_FILES}
)

target_link_libraries(SSevGui PRIVATE Qt5::Widgets)
target_link_libraries(SSevGui PRIVATE Qt5::Network)
target_link_libraries(SSevGui PRIVATE Qt5::DBus)
