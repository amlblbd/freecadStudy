cmake_minimum_required(VERSION 3.16)
project(freecadStudy)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加conan生成的工具链
if(EXISTS "${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
    include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
endif()

# 设置CMAKE_PREFIX_PATH以查找Qt
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${CMAKE_BINARY_DIR})

# 查找Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

# 添加可执行文件
file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(qt6_test_app ${SRC_FILES})

# 链接到Qt6组件
target_link_libraries(qt6_test_app Qt6::Core Qt6::Widgets Qt6::Gui)

# 设置输出目录为build
set_target_properties(qt6_test_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)