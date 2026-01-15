cmake_minimum_required(VERSION 3.20)
project(MyFreeCAD)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.6.2 CONFIG REQUIRED COMPONENTS Widgets)

add_executable(MyFreeCAD src/Gui/QtTest.cpp)

target_link_libraries(MyFreeCAD PRIVATE Qt6::Widgets)
