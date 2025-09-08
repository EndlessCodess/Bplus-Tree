#!/bin/bash

# 设置变量
BUILD_DIR="build"
BIN_DIR="bin"

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

case "$choice" in
    1) BUILD_TYPE="Debug"; EXECUTABLE_NAME="${BIN_DIR}/Debug/BplusTreeExe";;
    2) BUILD_TYPE="Release"; EXECUTABLE_NAME="${BIN_DIR}/Release/BplusTreeExe";;
    *) error_exit "Invalid choice! Please choose 1 or 2.";;
esac

# 1. 创建和清理构建目录
if [ -d "$BUILD_DIR" ]; then
    warning "Build directory already exists. Cleaning up..."
    rm -rf "$BUILD_DIR" || error_exit "Failed to clean build directory."
fi
mkdir "$BUILD_DIR" || error_exit "Failed to create build directory."

# 2. 进入构建目录
cd "$BUILD_DIR" || error_exit "Failed to enter build directory."

# 3. 配置 CMake 项目
cmake -S .. -B . -DCMAKE_BUILD_TYPE=${BUILD_TYPE} || error_exit "CMake configuration failed."

# 4. 编译
CPU_CORES=$(nproc)
make -j${CPU_CORES} || error_exit "Build failed."

# 5. 检查构建是否完成
cd ..
if [ ! -f "$EXECUTABLE_NAME" ]; then
    error_exit "Build file not found: $EXECUTABLE_NAME"
fi
success "Build completed successfully. Executable: $EXECUTABLE_NAME"

# 6. 运行测试
if [ -f "${BUILD_DIR}/hello_test" ]; then
    echo -e "${YELLOW}Running tests...${NC}"
    cd "$BUILD_DIR"
    ./hello_test || error_exit "Tests failed."
    success "All tests passed."
else
    warning "No test executable found. Skipping tests."
fi
