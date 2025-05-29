cmake_minimum_required(VERSION 3.16)
project(ToDo_List)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# wxWidgets の検索
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# nlohmann/json のセットアップ
include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

# メインアプリケーション
add_executable(ToDo_List
    cApp.cpp
    cApp.h
    MainWindow.cpp
    MainWindow.h
    Task.cpp
    Task.h
)

target_link_libraries(ToDo_List 
    ${wxWidgets_LIBRARIES}
    nlohmann_json::nlohmann_json
) 