cmake_minimum_required(VERSION 3.20)
project(MyFreeCAD)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 6.6.2 CONFIG REQUIRED COMPONENTS Widgets)

add_executable(MyFreeCAD src/Gui/QtTest.cpp)

set(myfreecad_qt_platform_plugins "")
if(TARGET Qt6::QXcbIntegrationPlugin)
    list(APPEND myfreecad_qt_platform_plugins Qt6::QXcbIntegrationPlugin)
endif()

target_link_libraries(MyFreeCAD PRIVATE ${myfreecad_qt_platform_plugins} Qt6::Widgets)
