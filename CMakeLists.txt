cmake_minimum_required(VERSION 3.25)
project(MyFreeCAD)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 生成 compile_commands.json，供 IDE/clangd 读取真实的编译参数（include 路径/宏/编译选项）
# 这样你在编辑器里看到的“波浪线报错”会显著减少
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT ProgramDatabase)
  add_compile_options($<$<CONFIG:Debug>:/Zi> $<$<CONFIG:Debug>:/Od>)
  add_link_options($<$<CONFIG:Debug>:/DEBUG>)
endif()


find_package(Qt6 CONFIG REQUIRED COMPONENTS Widgets)

# 最小可执行程序：用于验证 Qt6 是否能成功编译、链接与运行
add_executable(MyFreeCAD src/Gui/QtTest.cpp)

# 链接顺序很重要：把平台插件放在 Qt6::Widgets 前面，避免静态库符号在某些链接器/参数下解析不到
target_link_libraries(MyFreeCAD PRIVATE ${myfreecad_qt_platform_plugins} Qt6::Widgets)

include(GNUInstallDirs)

if(WIN32)
  if(TARGET Qt6::windeployqt)
    add_custom_command(
      TARGET MyFreeCAD
      POST_BUILD
      COMMAND Qt6::windeployqt --no-translations --no-compiler-runtime --no-opengl-sw $<TARGET_FILE:MyFreeCAD>
      COMMENT "Deploy Qt runtime and plugins"
    )
  else()
    add_custom_command(
      TARGET MyFreeCAD
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              $<TARGET_RUNTIME_DLLS:MyFreeCAD>
              $<TARGET_FILE_DIR:MyFreeCAD>
      COMMAND_EXPAND_LISTS
      COMMENT "Copy runtime DLLs for debugging"
    )

    get_filename_component(_qt_prefix "${Qt6_DIR}/../../.." ABSOLUTE)
    set(_qt_platform_plugin "${_qt_prefix}/plugins/platforms/qwindows.dll")
    if(EXISTS "${_qt_platform_plugin}")
      add_custom_command(
        TARGET MyFreeCAD
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:MyFreeCAD>/plugins/platforms"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${_qt_platform_plugin}"
                "$<TARGET_FILE_DIR:MyFreeCAD>/plugins/platforms"
        COMMENT "Copy Qt platform plugin"
      )
    endif()
  endif()
endif()

install(
  TARGETS MyFreeCAD
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(WIN32)
  set(MYFREECAD_TARGET MyFreeCAD)
  if(DEFINED WINDEPLOYQT_EXECUTABLE AND EXISTS "${WINDEPLOYQT_EXECUTABLE}")
    get_filename_component(_qt_bin "${WINDEPLOYQT_EXECUTABLE}" DIRECTORY)
    get_filename_component(MYFREECAD_QT_PREFIX "${_qt_bin}/.." ABSOLUTE)
  else()
    get_filename_component(MYFREECAD_QT_PREFIX "${Qt6_DIR}/../../.." ABSOLUTE)
  endif()
  set(_runtime_deps_script "${CMAKE_CURRENT_BINARY_DIR}/InstallRuntimeDeps.cmake")
  configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/cmake/InstallRuntimeDeps.cmake.in"
    "${_runtime_deps_script}"
    @ONLY
  )
  install(SCRIPT "${_runtime_deps_script}")

  install(
    DIRECTORY "${MYFREECAD_QT_PREFIX}/plugins/platforms"
    DESTINATION "${CMAKE_INSTALL_BINDIR}/plugins"
    FILES_MATCHING PATTERN "qwindows*.dll"
  )

  install(
    FILES "${CMAKE_CURRENT_LIST_DIR}/cmake/qt.conf.in"
    DESTINATION "${CMAKE_INSTALL_BINDIR}"
    RENAME "qt.conf"
  )
endif()
