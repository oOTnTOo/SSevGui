cmake_minimum_required(VERSION 3.5)

project(SSevGui LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
  sqprofile.cpp
  ConnectionInfoWgt.cpp
  Settings.cpp
  SubscriptionManagerWgt.cpp
  BusView.cpp
  SSevGUI.qrc
)

target_link_libraries(SSevGui PRIVATE Qt5::Widgets)
target_link_libraries(SSevGui PRIVATE Qt5::Network)
target_link_libraries(SSevGui PRIVATE Qt5::DBus)
