cmake_minimum_required(VERSION 3.13.4)
project(LLVMFuzzIntrospector)

# C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

# -------------------------------------------------------------------------
# 关键修改：使用 add_llvm_pass_plugin
# -------------------------------------------------------------------------
# 这个宏会自动处理：
# 1. 生成 .so 动态库
# 2. 避免链接宿主工具中已存在的静态库（解决 "Option registered more than once"）
# 3. 可以在 Linux 上允许未定义的符号（由 opt 在运行时提供）
# -------------------------------------------------------------------------

add_llvm_pass_plugin(FuzzIntrospector
  FuzzIntrospector.cpp

  LINK_COMPONENTS
  Core
  Support
  BinaryFormat
)