
# 设置Cmake最低版本
cmake_minimum_required(VERSION 3.16)
project(BplusTree)

# 设置C++版本
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++-14")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

# 启用 clang-tidy
set(CMAKE_CXX_CLANG_TIDY clang-tidy -p ${CMAKE_BINARY_DIR})


# 启用警告
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

# 设置源文件变量　
file(GLOB SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/include/*.h
)

# 添加可执行文件目录
add_executable(BplusTreeExe ${SRC_FILES})

# 添加头文件
#include_directories(${PROJECT_SOURCE_DIR}/include)
#add_compile_options(-I /usr/lib/gcc/x86_64-linux-gnu/12)
#target_include_directories(BplusTreeExe PRIVATE ${PROJECT_SOURCE_DIR}/include)
#target_include_directories(BplusTreeExe PRIVATE /usr/include/c++/12)
# 将项目头文件路径添加到目标
target_include_directories(BplusTreeExe PRIVATE ${PROJECT_SOURCE_DIR}/include)

# 将标准库的头文件路径添加到目标
target_include_directories(BplusTreeExe PRIVATE 
    /usr/include/c++/12
    /usr/include/x86_64-linux-gnu/c++/12
    /usr/lib/gcc/x86_64-linux-gnu/12/include
)

# 设置输出目录
set_target_properties(BplusTreeExe PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/Release
)

# 添加自定义目标以运行 clang-tidy
add_custom_target(run_clang_tidy
    COMMAND clang-tidy -p ${CMAKE_BINARY_DIR} ${SRC_FILES}
    COMMENT "Running clang-tidy on source files"
    VERBATIM)

# 确保在构建目标之前运行 clang-tidy
add_dependencies(BplusTreeExe run_clang_tidy)




