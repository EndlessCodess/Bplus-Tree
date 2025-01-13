
# 设置Cmake最低版本
cmake_minimum_required(VERSION 3.16)
project(BplusTree)

# 设置C++版本
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 启用警告
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

# 添加头文件
include_directories(${PROJECT_SOURCE_DIR}/include)

# 设置源文件变量
set(SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/BplusTree.cpp  
    ${PROJECT_SOURCE_DIR}/src/BNode.cpp
    ${PROJECT_SOURCE_DIR}/src/main.cpp
)

# 添加可执行文件目录
add_executable(BplusTreeExe ${SRC_FILES})

# 设置输出目录
set_target_properties(BplusTreeExe PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/Release
)


