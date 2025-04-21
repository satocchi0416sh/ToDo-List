cmake_minimum_required(VERSION 3.10)
project(ToDo_List)

# wxWidgetsを探す
find_package(wxWidgets REQUIRED COMPONENTS core base richtext)
include(${wxWidgets_USE_FILE})

# ソースファイルを追加
set(SOURCES
    cApp.cpp
    MainWindow.cpp
    Task.cpp
)

# 実行ファイルの設定
add_executable(ToDo_List ${SOURCES})

# wxWidgetsのライブラリをリンク
target_link_libraries(ToDo_List ${wxWidgets_LIBRARIES})

# C++11標準を使用
set_target_properties(ToDo_List PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
) 