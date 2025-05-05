cmake_minimum_required(VERSION 3.10)
project(NetworkTools)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ソースファイルの設定
set(SOURCES
    src/main.cpp
    src/network_manager.cpp
    src/utils.cpp
)

# インクルードディレクトリの設定
include_directories(${PROJECT_SOURCE_DIR}/src)

# 実行ファイルの生成
add_executable(network_tools ${SOURCES}) 