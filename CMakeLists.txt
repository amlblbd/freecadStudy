cmake_minimum_required(VERSION 3.25)
project(MyFreeCAD)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 生成 compile_commands.json，供 IDE/clangd 读取真实的编译参数（include 路径/宏/编译选项）
# 这样你在编辑器里看到的“波浪线报错”会显著减少
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_package(Qt6 6.6.2 CONFIG REQUIRED COMPONENTS Widgets)

# 最小可执行程序：用于验证 Qt6 是否能成功编译、链接与运行
add_executable(MyFreeCAD src/Gui/QtTest.cpp)

# # 平台插件处理（Linux 下通常用 xcb/wayland）：
# # 你当前这套 Conan Qt 很可能把平台插件做成“静态库”（plugins/platforms 里是 *.a）
# # 静态插件不会在运行时从磁盘加载 .so，因此需要：
# # 1) 程序里 Q_IMPORT_PLUGIN 静态导入
# # 2) 可执行文件链接上对应的插件 target
# set(myfreecad_qt_platform_plugins "")
# if(TARGET Qt6::QXcbIntegrationPlugin)
#     list(APPEND myfreecad_qt_platform_plugins Qt6::QXcbIntegrationPlugin)
# endif()

# 链接顺序很重要：把平台插件放在 Qt6::Widgets 前面，避免静态库符号在某些链接器/参数下解析不到
target_link_libraries(MyFreeCAD PRIVATE ${myfreecad_qt_platform_plugins} Qt6::Widgets)
