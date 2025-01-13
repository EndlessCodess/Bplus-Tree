#!/bin/bash

# 设置变量
BUILD_DIR="build"

# 设置颜色
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m'

# 打印函数
function error_exit {
    echo -e "${RED}[ERROR] $1${NC}"
    exit 1
}

function success {
    echo -e "${GREEN}[SUCCESS] $1${NC}"
}

function warning {
    echo -e "${YELLOW}[WARNING] $1${NC}"
}

# 提供选择菜单
echo -e "${YELLOW}Choose build type:${NC}"
echo "1. Debug"
echo "2. Release"
read -p "Enter choice (1/2): " choice

if [[ "$choice" == "1" ]]; then
    BUILD_TYPE="Debug"
    EXECUTABLE_NAME="bin/Debug/BplusTreeExe"
elif [[ "$choice" == "2" ]]; then
    BUILD_TYPE="Release"
    EXECUTABLE_NAME="bin/Release/BplusTreeExe"
else
    error_exit "Invalid choice! Please choose 1 or 2."
fi

# 1.创建和清理构建目录
if [ -d "$BUILD_DIR" ]; then
    warning "Build directory already exists."
    rm -rf "$BUILD_DIR" || error_exit "Failed to clean build directory."
fi 
mkdir "$BUILD_DIR" || error_exit "Failed to create build directory."

# 2.进入构建目录
cd "$BUILD_DIR" || error_exit "Failed to enter build directory."

# 3.配置CMake项目
cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} ..|| error_exit "CMake configuration failed."

# 4.编译
make || error_exit "Build failed."

# 5.检查构建是否完成
cd ..
if [ ! -f "$EXECUTABLE_NAME" ]; then
    error_exit "Build file not found."
fi
success "Build completed successfully."

