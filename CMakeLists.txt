cmake_minimum_required(VERSION 3.25)
project(MyFreeCAD)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 生成 compile_commands.json，供 IDE/clangd 读取真实的编译参数（include 路径/宏/编译选项）
# 这样你在编辑器里看到的“波浪线报错”会显著减少
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_package(Qt6 CONFIG REQUIRED COMPONENTS Widgets)

# 最小可执行程序：用于验证 Qt6 是否能成功编译、链接与运行
add_executable(MyFreeCAD src/Gui/QtTest.cpp)

# 链接顺序很重要：把平台插件放在 Qt6::Widgets 前面，避免静态库符号在某些链接器/参数下解析不到
target_link_libraries(MyFreeCAD PRIVATE ${myfreecad_qt_platform_plugins} Qt6::Widgets)
