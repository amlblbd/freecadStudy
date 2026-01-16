#!/usr/bin/env bash
set -euo pipefail

# 用法：
#   ./build.sh [Release|Debug] [build|run|clean|rebuild]
#
# 设计目标：
# - 一条命令完成 Conan 安装依赖 + CMake 配置 + 构建（可选运行）
# - 默认输出尽量简洁；遇到失败时给出日志路径与末尾关键日志
#
# 环境变量：
# - FREECADSTUDY_VERBOSE=1：显示完整 conan/cmake 输出（调试用）
# - CONAN_INSTALL_SYSTEM_DEPS=0：禁止 conan 通过 apt 安装系统依赖（无 sudo 环境时用）

build_type="${1:-Release}"
action="${2:-build}"
verbosity="${FREECADSTUDY_VERBOSE:-0}"

# CMake 预设由 Conan 的 CMakeToolchain 生成，命名一般是 conan-release / conan-debug
build_type_lower="$(printf '%s' "$build_type" | tr '[:upper:]' '[:lower:]')"
preset="conan-${build_type_lower}"

if [[ "$action" == "-h" || "$action" == "--help" || "$action" == "help" ]]; then
  printf '%s\n' "Usage: ./build.sh [Release|Debug] [build|run|clean|rebuild]"
  printf '%s\n' "Env:"
  printf '%s\n' "  FREECADSTUDY_VERBOSE=1          Show full conan/cmake logs"
  printf '%s\n' "  CONAN_INSTALL_SYSTEM_DEPS=0     Disable installing system deps"
  exit 0
fi

if [[ "$action" == "clean" ]]; then
  # 清理构建目录（不会动 Conan 缓存）
  rm -rf build
  exit 0
fi

if [[ "$action" == "rebuild" ]]; then
  # 先清理再构建
  rm -rf build
  action="build"
fi

# 自动检测/生成默认 Conan profile（包含编译器、架构、编译类型等）
# - 关闭输出是为了保持 build.sh 运行时更“安静”
if [[ "$verbosity" == "1" ]]; then
  conan profile detect --force
else
  conan profile detect --force >/dev/null 2>&1
fi

# Conan 安装系统依赖的控制：
# Qt 在 Linux 上通常会依赖一堆 X11/XCB/Wayland 开发包；让 Conan 代装能少踩坑
conan_conf=()
if [[ "${CONAN_INSTALL_SYSTEM_DEPS:-1}" == "1" ]]; then
  conan_conf+=(-c "tools.system.package_manager:mode=install" -c "tools.system.package_manager:sudo=True")
fi

# 约定 build/ 为构建目录，同时把 conan/cmake 的详细输出保存到日志文件里（默认不在终端刷屏）
mkdir -p build
conan_log="build/conan-install-${build_type_lower}.log"
cmake_log="build/cmake-configure-${build_type_lower}.log"

if [[ "$verbosity" == "1" ]]; then
  conan install . -of build -s "build_type=${build_type}" --build=missing "${conan_conf[@]}"
else
  printf '%s\n' "[1/3] Conan install (${build_type})..."
  : > "${conan_log}"
  if ! conan install . -of build -s "build_type=${build_type}" --build=missing "${conan_conf[@]}" >"${conan_log}" 2>&1; then
    printf '%s\n' "Conan install failed. Log: ${conan_log}"
    tail -n 80 "${conan_log}" || true
    exit 1
  fi
fi

if [[ "$verbosity" == "1" ]]; then
  cmake --preset "${preset}"
else
  printf '%s\n' "[2/3] CMake configure (${preset})..."
  : > "${cmake_log}"
  if ! cmake --preset "${preset}" >"${cmake_log}" 2>&1; then
    printf '%s\n' "CMake configure failed. Log: ${cmake_log}"
    tail -n 80 "${cmake_log}" || true
    exit 1
  fi
fi

printf '%s\n' "[3/3] CMake build..."
# 让 Makefiles 构建更安静（-s），只保留必要的错误信息
cmake --build --preset "${preset}" -j -- -s

if [[ "$action" == "run" ]]; then
  # conanrun.sh 可能引用一些未定义环境变量；临时关闭 set -u 避免 “unbound variable”
  set +u
  source ./build/conanrun.sh
  set -u
  ./build/MyFreeCAD
fi
