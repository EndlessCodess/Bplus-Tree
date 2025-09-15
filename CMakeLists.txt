# 设置 CMake 最低版本
cmake_minimum_required(VERSION 3.16)
project(BplusTree)

# 设置 C++ 版本
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 强制 Debug 模式添加调试符号并禁用优化
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0" CACHE STRING "Debug build flags" FORCE)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0" CACHE STRING "Debug build flags" FORCE)

# 启用警告
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

# 启用 clang-tidy（如果可用）
if(CMAKE_CXX_CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY clang-tidy -p ${CMAKE_BINARY_DIR})
endif()

# 设置源文件和头文件目录
set(TEST_DIR ${PROJECT_SOURCE_DIR}/test)
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

# 收集源文件和头文件
file(GLOB SRC_FILES
    "${SOURCE_DIR}/*.cpp"
)
file(GLOB HEADER_FILES
    "${INCLUDE_DIR}/*.h"
)
# 编译 BplusTree 库（仅头文件库，无需 .cpp）
add_library(BplusTree INTERFACE)
target_include_directories(BplusTree INTERFACE ${INCLUDE_DIR})

# 添加可执行文件
# add_executable(BplusTreeExe ${SOURCE_DIR}/main.cpp)
add_executable(BplusTreeExe ${TEST_DIR}/test7.cpp)
target_include_directories(BplusTreeExe PRIVATE ${INCLUDE_DIR})
target_link_libraries(BplusTreeExe PRIVATE BplusTree)

# 设置输出目录
set_target_properties(BplusTreeExe PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/Release
)

# 引入本地 Google Test（保留以支持未来测试）
add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/googletest)

# 启用 CMake 的测试功能
enable_testing()

# 添加自定义目标以运行 clang-tidy
add_custom_target(run_clang_tidy
    COMMAND clang-tidy -p ${CMAKE_BINARY_DIR} ${SRC_FILES} ${HEADER_FILES}
    COMMENT "Running clang-tidy on source and header files"
    VERBATIM
)

# 确保在构建目标之前运行 clang-tidy
add_dependencies(BplusTreeExe run_clang_tidy)

# 添加测试子目录（如果有测试文件）
# add_subdirectory(test)